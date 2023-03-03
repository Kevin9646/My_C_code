#include "sys.h"
#include "usart.h"	



u8 *pTxBuffer;//发送数据缓存指针
u8 *pTxBuffer3;//发送数据缓存指针
u8 TxLen=0;//发送数据长度
u8 TxLen2=0;//发送数据长度
u8 TxLen3=0;//发送数据长度


/* 舵机相关变量 */

u8 USART_RX_BUF[16];
u8 blen=0;                      /* zyq */
bool isUartRxCompleted = false;


void USART1_Init(u32 bound){
   //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //使能GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//使能USART1时钟
 
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); //GPIOA9复用为USART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA10复用为USART1
	
	//USART1端口配置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //GPIOA9与GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure); //初始化PA9，PA10

   //USART1 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
  USART_Init(USART1, &USART_InitStructure); //初始化串口1
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启接受中断
	USART_ITConfig(USART1, USART_IT_TC, ENABLE);//开启发送中断
	USART_Cmd(USART1, ENABLE);                    //使能串口

	//Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//子优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、
}





/********************************************************************
函数功能：Usart发送数字(范围0~9)。
入口参数：Num: 要发送的字节数据。
返    回：无。
备    注：无。
********************************************************************/
void Usart_Tx_Num(unsigned char Num)
{
	USART1->SR;//发送之前读取SR寄存器可以避免发送时，第一字节丢失的bug
	USART_SendData(USART1,Num+0x30);			//写入数据
	while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);	//等待发送完成
	USART_ClearFlag(USART1, USART_FLAG_TC);//清除发送完成标志
}
////////////////////////End of function//////////////////////////////


/********************************************************************
函数功能：Usart发送一字节数据。
入口参数：Char: 要发送的字节数据。
返    回：无。
备    注：无。
********************************************************************/
void Usart_Tx_Char(unsigned char Char)
{
	USART1->SR;//发送之前读取SR寄存器可以避免发送时，第一字节丢失的bug
	USART_SendData(USART1,Char);			//写入数据
	while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);	//等待发送完成
	USART_ClearFlag(USART1, USART_FLAG_TC);//清除发送完成标志
}
////////////////////////End of function//////////////////////////////


/********************************************************************
函数功能：Usart发送一个字符串。
入口参数：Str：要发送的字符串指针。
返    回：无。
备    注：无。
********************************************************************/
void Usart_Tx_String(unsigned char *Str)
{
	USART1->SR;//发送之前读取SR寄存器可以避免发送时，第一字节丢失的bug
	while(*Str!='\0')                           //发送字符串，直到遇到字符串结束符才结束                        
	{		
		USART_SendData(USART1,*Str);			//写入数据
		while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);	//等待发送完成
		USART_ClearFlag(USART1, USART_FLAG_TC);//清除发送完成标志
		Str++;	                                //移动到下一个字符
	}
}
////////////////////////End of function//////////////////////////////

/**************************************************************************
函数功能：连续发送数据；
入口参数：u8 *pData 数据存储的地址 ,u8 Len 数据长度；
返回参数：无；
备    注：TxLen在中断函数中继续处理 ； 
**************************************************************************/
void USART1_SendDataString( u8 *pData ,u8 Len)
{
//	RS485_Control = 1;
//	RS485_LIM_TIM(0) = -1;								/* 485保护关闭 */
	pTxBuffer = pData;
	TxLen = Len;
	USART_ClearFlag(USART1, USART_FLAG_TC);
	USART_SendData(USART1, *(pTxBuffer++));
	TxLen--;
}
////////////////////////End of function//////////////////////////////

void USART1_IRQHandler(void)                	//串口1中断服务程序
{
	u8 res=0;
	
	
	/* 解决卡中断问题 START */
	if (USART_GetFlagStatus(USART1, USART_FLAG_ORE) != RESET)
	 {
        USART_ReceiveData(USART1);
   }
	/*  END */
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)//接收中断
	{ 
		res=USART_ReceiveData(USART1);				//读取数据
		ProtRecByte(res);											//对数据根据协议散转
	}
	if (USART_GetITStatus(USART1, USART_IT_TC) == SET)
	{
		if(TxLen==0)
		{
				USART_ClearFlag(USART1, USART_FLAG_TC);
		}
		else
		{
				USART_SendData(USART1,*pTxBuffer++);//发送数据
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
 

/* 舵机相关start */

void USART2_Init(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
  RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOA, ENABLE);

  /* 使能 UART 时钟 */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
  
  /* 连接 PXx 到 USARTx_Tx*/
  GPIO_PinAFConfig(GPIOA,GPIO_PinSource3, GPIO_AF_USART2);

  /*  连接 PXx 到 USARTx__Rx*/
  GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2);

  /* 配置Tx引脚为复用功能  */
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2  ;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* 配置Rx引脚为复用功能 */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
			
  /* 配置串口RS232_USART 模式 */
  USART_InitStructure.USART_BaudRate = bound;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No ;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART2, &USART_InitStructure); 
	
		//Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;//抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//子优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、
	

	/*配置串口接收中断*/
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
//	USART_ITConfig(USART2, USART_IT_TC, ENABLE);//开启发送中断
  USART_Cmd(USART2, ENABLE);
//	USART_ClearFlag(USART2, USART_FLAG_TC);
}





void uartWriteBuf(uint8_t *buf, uint8_t len)
{
	TxLen2 = len;
	
	LOBOT_TXEN=1;															/* 硬件发送使能读取失能 */
	LOBOT_RXEN=0;
	
	while (len--)
	{
		while ((USART2->SR & 0x40) == 0);

		USART_SendData(USART2,*buf++);
	}
	
	delay_us(150);														/* 等待最后一个数据发送完成 */
	
	LOBOT_TXEN=0;														/* 硬件读取使能发送失能 */
	LOBOT_RXEN=1;
}



void USART2_IRQHandler(void)
{
	uint8_t Res;
	static bool isGotFrameHeader1 = false;
	static bool isGotFrameHeader = false;
	
		/* 解决卡中断问题 START */
	if (USART_GetFlagStatus(USART2, USART_FLAG_ORE) != RESET)
	 {
        USART_ReceiveData(USART2);
   }
	/*  END */
	
	
	if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{ //判断接收中断
		Res = USART_ReceiveData(USART2);//(USART1->DR);	//取出接收寄存器数据		
				// 大叉
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
//		//小叉
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



/*角度传感器*/

void USART3_Init(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

  /* 使能 UART 时钟 */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
  
  /* 连接 PXx 到 USARTx_Tx*/
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource8, GPIO_AF_USART3);

  /*  连接 PXx 到 USARTx__Rx*/
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource9,GPIO_AF_USART3);

  /* 配置Tx引脚为复用功能  */
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8  ;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  /* 配置Rx引脚为复用功能 */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
			
  /* 配置串口USART 模式 */
  USART_InitStructure.USART_BaudRate = bound;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No ;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART3, &USART_InitStructure); 
	
		//Usart3 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;//抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;		//子优先级1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、
	

	/*配置串口接收中断*/
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART3, USART_IT_TC, ENABLE);//开启发送中断
  USART_Cmd(USART3, ENABLE);
}

/**************************************************************************
函数功能：连续发送数据；
入口参数：u8 *pData 数据存储的地址 ,u8 Len 数据长度；
返回参数：无；
备    注：TxLen在中断函数中继续处理 ； 
**************************************************************************/
void USART3_SendDataString( u8 *pData ,u8 Len)
{
	RS485_Control = 1;
//	RS485_LIM_TIM(0) = -1;								/* 485保护关闭 */
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
		/* 解决卡中断问题 START */
	if (USART_GetFlagStatus(USART3, USART_FLAG_ORE) != RESET)
	 {
        USART_ReceiveData(USART3);
   }
	/*  END */
	
	if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{ //判断接收中断
		Res = USART_ReceiveData(USART3);//(USART3->DR);	//取出接收寄存器数据
		ProtRecByte3(Res);
	}
	if (USART_GetITStatus(USART3, USART_IT_TC) == SET)
	{
		if(TxLen3==0)
		{
				RS485_Control = 0;
//				RS485_LIM_TIM(0) = 0;													/* 正常情况下，提前取消软件定时器 */
				USART_ClearFlag(USART3, USART_FLAG_TC);
		}
		else
		{
				USART_SendData(USART3,*pTxBuffer3++);//发送数据
				TxLen3--;
		}
	}	
}

void Read_Rotate_Angle(void)
{
	static unsigned char buffer[8] = {0x01, 0x03, 0x00, 0x03, 0x00, 0x01, 0x74, 0x0A};
	USART3_SendDataString(buffer, 8);
}



