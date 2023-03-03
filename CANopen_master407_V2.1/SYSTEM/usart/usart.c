#include "sys.h"
#include "usart.h"	



u8 *pTxBuffer;//�������ݻ���ָ��
u8 *pTxBuffer3;//�������ݻ���ָ��
u8 TxLen=0;//�������ݳ���
u8 TxLen2=0;//�������ݳ���
u8 TxLen3=0;//�������ݳ���


/* �����ر��� */

u8 USART_RX_BUF[16];
u8 blen=0;                      /* zyq */
bool isUartRxCompleted = false;


void USART1_Init(u32 bound){
   //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //ʹ��GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//ʹ��USART1ʱ��
 
	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); //GPIOA9����ΪUSART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA10����ΪUSART1
	
	//USART1�˿�����
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //GPIOA9��GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOA,&GPIO_InitStructure); //��ʼ��PA9��PA10

   //USART1 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
  USART_Init(USART1, &USART_InitStructure); //��ʼ������1
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//���������ж�
	USART_ITConfig(USART1, USART_IT_TC, ENABLE);//���������ж�
	USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ���

	//Usart1 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//��ռ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//�����ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����
}





/********************************************************************
�������ܣ�Usart��������(��Χ0~9)��
��ڲ�����Num: Ҫ���͵��ֽ����ݡ�
��    �أ��ޡ�
��    ע���ޡ�
********************************************************************/
void Usart_Tx_Num(unsigned char Num)
{
	USART1->SR;//����֮ǰ��ȡSR�Ĵ������Ա��ⷢ��ʱ����һ�ֽڶ�ʧ��bug
	USART_SendData(USART1,Num+0x30);			//д������
	while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);	//�ȴ��������
	USART_ClearFlag(USART1, USART_FLAG_TC);//���������ɱ�־
}
////////////////////////End of function//////////////////////////////


/********************************************************************
�������ܣ�Usart����һ�ֽ����ݡ�
��ڲ�����Char: Ҫ���͵��ֽ����ݡ�
��    �أ��ޡ�
��    ע���ޡ�
********************************************************************/
void Usart_Tx_Char(unsigned char Char)
{
	USART1->SR;//����֮ǰ��ȡSR�Ĵ������Ա��ⷢ��ʱ����һ�ֽڶ�ʧ��bug
	USART_SendData(USART1,Char);			//д������
	while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);	//�ȴ��������
	USART_ClearFlag(USART1, USART_FLAG_TC);//���������ɱ�־
}
////////////////////////End of function//////////////////////////////


/********************************************************************
�������ܣ�Usart����һ���ַ�����
��ڲ�����Str��Ҫ���͵��ַ���ָ�롣
��    �أ��ޡ�
��    ע���ޡ�
********************************************************************/
void Usart_Tx_String(unsigned char *Str)
{
	USART1->SR;//����֮ǰ��ȡSR�Ĵ������Ա��ⷢ��ʱ����һ�ֽڶ�ʧ��bug
	while(*Str!='\0')                           //�����ַ�����ֱ�������ַ����������Ž���                        
	{		
		USART_SendData(USART1,*Str);			//д������
		while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);	//�ȴ��������
		USART_ClearFlag(USART1, USART_FLAG_TC);//���������ɱ�־
		Str++;	                                //�ƶ�����һ���ַ�
	}
}
////////////////////////End of function//////////////////////////////

/**************************************************************************
�������ܣ������������ݣ�
��ڲ�����u8 *pData ���ݴ洢�ĵ�ַ ,u8 Len ���ݳ��ȣ�
���ز������ޣ�
��    ע��TxLen���жϺ����м������� �� 
**************************************************************************/
void USART1_SendDataString( u8 *pData ,u8 Len)
{
//	RS485_Control = 1;
//	RS485_LIM_TIM(0) = -1;								/* 485�����ر� */
	pTxBuffer = pData;
	TxLen = Len;
	USART_ClearFlag(USART1, USART_FLAG_TC);
	USART_SendData(USART1, *(pTxBuffer++));
	TxLen--;
}
////////////////////////End of function//////////////////////////////

void USART1_IRQHandler(void)                	//����1�жϷ������
{
	u8 res=0;
	
	
	/* ������ж����� START */
	if (USART_GetFlagStatus(USART1, USART_FLAG_ORE) != RESET)
	 {
        USART_ReceiveData(USART1);
   }
	/*  END */
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)//�����ж�
	{ 
		res=USART_ReceiveData(USART1);				//��ȡ����
		ProtRecByte(res);											//�����ݸ���Э��ɢת
	}
	if (USART_GetITStatus(USART1, USART_IT_TC) == SET)
	{
		if(TxLen==0)
		{
				USART_ClearFlag(USART1, USART_FLAG_TC);
		}
		else
		{
				USART_SendData(USART1,*pTxBuffer++);//��������
				TxLen--;
		}
	}		 
} 

void rs485_contral_lim(void)
{
	if(RS485_LIM_TIM(1) == -1)
	{
		RS485_Control = 0;
		USART_ClearITPendingBit(USART1,USART_IT_ORE);    
		USART_ReceiveData( USART1 );
		RS485_LIM_TIM(0) = 0;
	}
}
 

/* ������start */

void USART2_Init(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
  RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOA, ENABLE);

  /* ʹ�� UART ʱ�� */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
  
  /* ���� PXx �� USARTx_Tx*/
  GPIO_PinAFConfig(GPIOA,GPIO_PinSource3, GPIO_AF_USART2);

  /*  ���� PXx �� USARTx__Rx*/
  GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2);

  /* ����Tx����Ϊ���ù���  */
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2  ;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* ����Rx����Ϊ���ù��� */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
			
  /* ���ô���RS232_USART ģʽ */
  USART_InitStructure.USART_BaudRate = bound;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No ;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART2, &USART_InitStructure); 
	
		//Usart1 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;//����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;//��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//�����ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����
	

	/*���ô��ڽ����ж�*/
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
//	USART_ITConfig(USART2, USART_IT_TC, ENABLE);//���������ж�
  USART_Cmd(USART2, ENABLE);
//	USART_ClearFlag(USART2, USART_FLAG_TC);
}





void uartWriteBuf(uint8_t *buf, uint8_t len)
{
	TxLen2 = len;
	
	LOBOT_TXEN=1;															/* Ӳ������ʹ�ܶ�ȡʧ�� */
	LOBOT_RXEN=0;
	
	while (len--)
	{
		while ((USART2->SR & 0x40) == 0);

		USART_SendData(USART2,*buf++);
	}
	
	delay_us(150);														/* �ȴ����һ�����ݷ������ */
	
	LOBOT_TXEN=0;														/* Ӳ����ȡʹ�ܷ���ʧ�� */
	LOBOT_RXEN=1;
}



void USART2_IRQHandler(void)
{
	uint8_t Res;
	static bool isGotFrameHeader1 = false;
	static bool isGotFrameHeader = false;
	
		/* ������ж����� START */
	if (USART_GetFlagStatus(USART2, USART_FLAG_ORE) != RESET)
	 {
        USART_ReceiveData(USART2);
   }
	/*  END */
	
	
	if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{ //�жϽ����ж�
		Res = USART_ReceiveData(USART2);//(USART1->DR);	//ȡ�����ռĴ�������		
				// ���
		if(Res==0xFF)
		{
			isGotFrameHeader1=true;
		}
		if(isGotFrameHeader1 && (blen<8))
		{
			USART_RX_BUF[blen]=Res;
			blen++;
			if(blen == 8)
			{
				memcpy(LobotRxBuf, USART_RX_BUF, blen);	
				isUartRxCompleted = true;
				isGotFrameHeader1 = false;
				blen=0;
			}
		}
//		//С��
//		if(Res==0x55)
//		{
//			isGotFrameHeader=true;
//		}
//		if(isGotFrameHeader&&(blen<8))
//		{
//			USART_RX_BUF[blen]=Res;
//			blen++;
//			if(blen == 8)
//			{
//				memcpy(LobotRxBuf, USART_RX_BUF, blen);	
//				isUartRxCompleted = true;
//				isGotFrameHeader = false;
//				blen=0;
//			}
//		}
	}
	if (USART_GetITStatus(USART2, USART_IT_TC) == SET)
	{
		if(TxLen2==0)
		{
				USART_ClearFlag(USART2, USART_FLAG_TC);
				LOBOT_TXEN=0;
				LOBOT_RXEN=1;	
		}
		else
		{
			TxLen2--;
		}
	}
}

bool isRxCompleted(void)
{
	if(isUartRxCompleted == true)
	{
		isUartRxCompleted = false;
		return true;
	}
	else
	{
		return false;
	}
}



/*�Ƕȴ�����*/

void USART3_Init(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

  /* ʹ�� UART ʱ�� */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
  
  /* ���� PXx �� USARTx_Tx*/
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource8, GPIO_AF_USART3);

  /*  ���� PXx �� USARTx__Rx*/
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource9,GPIO_AF_USART3);

  /* ����Tx����Ϊ���ù���  */
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8  ;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  /* ����Rx����Ϊ���ù��� */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
			
  /* ���ô���USART ģʽ */
  USART_InitStructure.USART_BaudRate = bound;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No ;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART3, &USART_InitStructure); 
	
		//Usart3 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;//����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;//��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;		//�����ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����
	

	/*���ô��ڽ����ж�*/
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART3, USART_IT_TC, ENABLE);//���������ж�
  USART_Cmd(USART3, ENABLE);
}

/**************************************************************************
�������ܣ������������ݣ�
��ڲ�����u8 *pData ���ݴ洢�ĵ�ַ ,u8 Len ���ݳ��ȣ�
���ز������ޣ�
��    ע��TxLen���жϺ����м������� �� 
**************************************************************************/
void USART3_SendDataString( u8 *pData ,u8 Len)
{
	RS485_Control = 1;
//	RS485_LIM_TIM(0) = -1;								/* 485�����ر� */
	pTxBuffer3 = pData;
	TxLen3 = Len;
	USART_ClearFlag(USART3, USART_FLAG_TC);
	USART_SendData(USART3, *(pTxBuffer3++));
	TxLen3--;
}
////////////////////////End of function//////////////////////////////

void USART3_IRQHandler(void)
{
	uint8_t Res;
		/* ������ж����� START */
	if (USART_GetFlagStatus(USART3, USART_FLAG_ORE) != RESET)
	 {
        USART_ReceiveData(USART3);
   }
	/*  END */
	
	if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{ //�жϽ����ж�
		Res = USART_ReceiveData(USART3);//(USART3->DR);	//ȡ�����ռĴ�������
		ProtRecByte3(Res);
	}
	if (USART_GetITStatus(USART3, USART_IT_TC) == SET)
	{
		if(TxLen3==0)
		{
				RS485_Control = 0;
//				RS485_LIM_TIM(0) = 0;													/* ��������£���ǰȡ�������ʱ�� */
				USART_ClearFlag(USART3, USART_FLAG_TC);
		}
		else
		{
				USART_SendData(USART3,*pTxBuffer3++);//��������
				TxLen3--;
		}
	}	
}

void Read_Rotate_Angle(void)
{
	static unsigned char buffer[8] = {0x01, 0x03, 0x00, 0x03, 0x00, 0x01, 0x74, 0x0A};
	USART3_SendDataString(buffer, 8);
}



