// Includes for the Canfestival driver
#include "CanopenCan.h"

//Initialize the CAN hardware 
unsigned char CAN1_Init(CO_Data * d, uint32_t bitrate)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  NVIC_InitTypeDef  NVIC_InitStructure;
  CAN_InitTypeDef        CAN_InitStructure;
  CAN_FilterInitTypeDef  CAN_FilterInitStructure;

  /* CAN GPIOs configuration **************************************************/

  /* Enable GPIO clock */
  RCC_AHB1PeriphClockCmd(CAN_GPIO_CLK, ENABLE);

  /* Connect CAN pins to AF7 */
  GPIO_PinAFConfig(CAN_GPIO_PORT, CAN_RX_SOURCE, GPIO_AF_CANx);
  GPIO_PinAFConfig(CAN_GPIO_PORT, CAN_TX_SOURCE, GPIO_AF_CANx); 
  
  /* Configure CAN RX and TX pins */
  GPIO_InitStructure.GPIO_Pin = CAN_RX_PIN | CAN_TX_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(CAN_GPIO_PORT, &GPIO_InitStructure);

  /* NVIC configuration *******************************************************/
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);	
  NVIC_InitStructure.NVIC_IRQChannel = CANx_RX0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x3;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x3;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  /* CAN configuration ********************************************************/  
  /* Enable CAN clock */
  RCC_APB1PeriphClockCmd(CAN_CLK, ENABLE);
  
  /* CAN register init */
  CAN_DeInit(CANx);
  CAN_StructInit(&CAN_InitStructure);

  /* CAN cell init */
  CAN_InitStructure.CAN_TTCM = DISABLE;
  CAN_InitStructure.CAN_ABOM = ENABLE;
  CAN_InitStructure.CAN_AWUM = DISABLE;
  CAN_InitStructure.CAN_NART = DISABLE;
  CAN_InitStructure.CAN_RFLM = DISABLE;
  CAN_InitStructure.CAN_TXFP = DISABLE;
  CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
  CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
    
  /* CAN Baudrate (CAN clocked at 42 MHz)  42e6 / ( 6 * (1+BS1+BS2))  */
  CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
  if(bitrate == 1000000){
  	CAN_InitStructure.CAN_BS1 = CAN_BS1_3tq;
  	CAN_InitStructure.CAN_BS2 = CAN_BS2_3tq;
  }
  else {	//除去1M频率。剩下都配置为500K
  	CAN_InitStructure.CAN_BS1 = CAN_BS1_6tq;
  	CAN_InitStructure.CAN_BS2 = CAN_BS2_7tq;
  }

  CAN_InitStructure.CAN_Prescaler = 6;
  CAN_Init(CANx, &CAN_InitStructure);

  /* CAN filter init */
  CAN_FilterInitStructure.CAN_FilterNumber = 0;
  CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
  CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
  CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;
  CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
  CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;
  CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
  CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;
  CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
  CAN_FilterInit(&CAN_FilterInitStructure);
  
  /* Enable FIFO 0 message pending Interrupt */
  CAN_ITConfig(CANx, CAN_IT_FMP0, ENABLE);

  return 1;
}

// The driver send a CAN message passed from the CANopen stack
uint8_t canSend(CAN_PORT notused, Message *message) 
{
	uint32_t i = 0xFFFF;																		/* 配置can发送阻塞时间，如果在规定时间内未完成can发送，可配置报错 */
	CanTxMsg TxMessage;
	uint8_t TransmitMailbox = 0;

	/* 组装CAN数据包 */
	TxMessage.DLC = message->len;							/* 数据长度 */
	memcpy(TxMessage.Data, message->data, message->len);	/* 数据 */
	TxMessage.IDE = CAN_ID_STD;								/* 标准ID */
	TxMessage.StdId = message->cob_id;						/* 标识符 */
	TxMessage.RTR = (message->rtr == CAN_RTR_DATA) ? 0 : 2;	/* 数据帧 */

	/* 发送数据包 */
	TransmitMailbox = CAN_Transmit(CAN1, &TxMessage);
	/* 等待发送成功 */
	while((CAN_TransmitStatus(CAN1, TransmitMailbox) != CANTXOK) && --i);

	/* 成功0 超时1 */
	return (i != 0) ? 0 : 1; 
}

//The driver pass a received CAN message to the stack
/*
unsigned char canReceive(Message *m)
{
}
*/
unsigned char canChangeBaudRate_driver( CAN_HANDLE fd, char* baud)
{
	return 0;
}

/**
  * @brief  This function handles CAN1 RX0 interrupt request.
  * @param  None
  * @retval None
  */
void CAN1_RX0_IRQHandler(void)
{
	CanRxMsg message;
	Message Rx_Message;

	/* 接收消息 */
	CAN_Receive(CAN1, CAN_FIFO0, &message);

	/* 组装canopen数据包 */
	Rx_Message.cob_id = message.StdId;						/* 功能码和节点ID */
	Rx_Message.rtr = (message.RTR == CAN_RTR_DATA) ? 0 : 1;	/* 标识符类型 */
	Rx_Message.len = message.DLC;							/* 数据包长度 */
	memcpy(Rx_Message.data, message.Data, message.DLC);		/* 数据 */

	/* canopen数据包分配处理函数 */
	canDispatch(&Master_Data, &Rx_Message);
}

