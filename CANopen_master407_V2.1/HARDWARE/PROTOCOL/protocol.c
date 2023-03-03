/**
  ******************************************************************************
  * @file    USER/protocol.c 
  * @author  Ruiqing.Fu
  * @version V0.0.1
  * @date    September-09-2013
  * @brief   Main Protocol Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2013 SIAT</center></h2>
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/

#include "protocol.h"
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static unsigned char gDecAddr;
static unsigned char gAddrLen[255] = {0};
static unsigned int gAddrLenSum;
static unsigned int * gProtData;
static void(**gProtWrite)(unsigned char addrH,unsigned char addrL,unsigned int val);
static unsigned int(**gProtRead)(int);
static unsigned int *gProtBuffer;
static unsigned char *gProtWriteFlag;
unsigned char SoftwareError;
extern unsigned char HardwareErrorL;
extern unsigned char HardwareErrorH;
static unsigned char ErrorCode;
u16 angle = 0;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/


void SetDecAddr(unsigned char decAddr)
{
	gDecAddr = decAddr;
}

void SetAddrLenght(unsigned char addrH,unsigned char len)
{
	gAddrLen[addrH] = len;
}

int GetLineAddrByAddr(unsigned char addrH,unsigned char addrL)
{
	int i;
	unsigned int sum =0;
	for(i = 0;i<addrH;i++)
	{
		sum += gAddrLen[i];
	}
	sum += addrL;
	return sum;
}

unsigned char GetAddrHByLineAddr(int lineAddr)
{
		unsigned char i;
		unsigned int sum = 0;
		for(i = 0;i<255;i++)
		{
				sum += gAddrLen[i];
			if(sum > lineAddr)
			{
				return i;
			}
		}
		return 0;
}

unsigned char GetAddrLByLineAddr(int lineAddr)
{
		unsigned char i;
		unsigned int sum = 0;
		for(i = 0;i<255;i++)
		{
			if(sum + gAddrLen[i] > lineAddr)
			{
				return lineAddr - sum;
			}
			sum += gAddrLen[i];
		}
		return 0;
}

void DefaultWriteFunc(unsigned char addrH,unsigned char addrL,unsigned int val)
{
	int lineAddr = GetLineAddrByAddr(addrH,addrL);
	gProtData[lineAddr] = val;
}

unsigned int DefaultReadLineAddrFunc(int lineAddr)
{
        return gProtData[lineAddr];
}

unsigned int DefaultReadFunc(unsigned char addrH,unsigned char addrL)
{
	int lineAddr = GetLineAddrByAddr(addrH,addrL);
	return gProtData[lineAddr];
}

void SetWriteFunc(unsigned char addrH,unsigned char addrL, void (* func)(unsigned char,unsigned char,unsigned int))
{
	int lineAddr = GetLineAddrByAddr(addrH,addrL);
	gProtWrite[lineAddr]=func;
}

void SetReadFunc(unsigned char addrH,unsigned char addrL,unsigned int(* func)(int))
{
	int lineAddr = GetLineAddrByAddr(addrH,addrL);
	gProtRead[lineAddr]=func;
}

void DoInitProt(void)
{
	int i;
	unsigned int sum = 0;
	SetAddrLenght(0x80,2);
	for(i=0;i<256;i++)
	{
		sum += gAddrLen[i];
	}
	gProtData = (unsigned int*)malloc(sum * sizeof(unsigned int));
	gProtWrite = (void (**)(unsigned char,unsigned char,unsigned int))
				malloc(sum * sizeof(void (*)(unsigned char,unsigned char,unsigned int)));
	gProtRead = (unsigned int(**)(int))
				malloc(sum * sizeof(unsigned int(*)(int)));
	gProtBuffer = (unsigned int*)malloc(sum * sizeof(unsigned int));
	gProtWriteFlag = (unsigned char*)malloc(sum * sizeof(unsigned char));
	gAddrLenSum = sum;
	for(i = 0;i<gAddrLenSum;i++)
	{
			gProtWrite[i] = DefaultWriteFunc;
			gProtRead[i] = DefaultReadLineAddrFunc;
			gProtData[i] = 0;
			gProtWriteFlag[i] = 0;
	}
}

void SendReadAck(unsigned char addrH,unsigned char addrL,unsigned char len)
{
	static unsigned char tempBuffer[100];
	unsigned char checkSum = 0;
	unsigned char addr = GetLineAddrByAddr(addrH,addrL);
	unsigned char i;
        unsigned char t;
	unsigned int tempData;
        unsigned char *p;
	tempBuffer[0] = 0x66;						//包头
        tempBuffer[1] = 0x00;						//读指令
	tempBuffer[2] = gDecAddr;				//本机地址
        tempBuffer[3] = addrL;                                  //设备低位地址
        tempBuffer[4] = addrH;                                  //设备高位地址
	tempBuffer[5] = len;						//长度
        len = len >> 1;
        p = tempBuffer + 6;
	for(i=0;i<len;i++)
	{
                t = i+addr;
		tempData = (*gProtRead[t])(t);
		*(p++) = (unsigned char)tempData;
		*(p++) = (unsigned char)(tempData >> 8);
	}
        len = len << 1;
        tempBuffer[len+6]=SoftwareError;
        tempBuffer[len+7]=0;
        tempBuffer[len+8]=0;
        t = len + 8;
        p = tempBuffer;
	for(i=0;i<t;i++)
	{
		checkSum += *(++p);
	}
	tempBuffer[len+9] = ~checkSum;
	tempBuffer[len+10] = 0xAA;
	USART1_SendDataString(tempBuffer,len+11);
}
void SendWriteAck(unsigned char addrH,unsigned char addrL)
{
        static unsigned char * tempBuffer1 = 0;
        int i=0;
        unsigned char checkSum = 0;
        if(tempBuffer1 != 0) free(tempBuffer1);
        tempBuffer1 = malloc(sizeof(unsigned char) * (7));
        tempBuffer1[0] = 0x66;						//包头
        tempBuffer1[1] = 0x01;						//写指令
	tempBuffer1[2] = gDecAddr;				//本机地址
        tempBuffer1[3] = addrL;                                  //设备低位地址
        tempBuffer1[4] = addrH;                                  //设备高位地址
	for(i=0;i<4;i++)
	{
		checkSum += tempBuffer1[i+1];
	}
        tempBuffer1[5]=~checkSum;
        tempBuffer1[6]=0xAA;
        USART1_SendDataString(tempBuffer1,7);
}

void ProtRecByte3(unsigned char rec)
{
  static unsigned char buffer3[7];
  static unsigned char count3 = 0;
  if(count3 == 0)
  {
    if(rec == 0x01) count3++;
  }
	else if(count3 == 1)
	{
		if(rec == 0x03) count3++;
	}
	else if(count3 == 2)
	{
		if(rec == 0x02) count3++;
	}
  else if(count3 == 3)
  {
    buffer3[0] = rec;
    count3++;
  }
  else if(count3 == 4)
  {
		buffer3[1] = rec;
		angle = buffer3[1] + ((u16)buffer3[0]<<8);
		count3++;
  }
  else if(count3 == 5)
  {
		count3++; // 暂时没有验证校验位
  }
	else if(count3 == 6)
	{
		count3 = 0;
	}
}


void ProtRecByte(unsigned char rec)
{
		//状态机接收
	static unsigned char buffer[80];
	static unsigned char count = 0;
	static unsigned char lenght = 0;
	static unsigned char decAddr = 0;
	static unsigned char cmdType = 0;
	static unsigned char addrH = 0;
	static unsigned char addrL = 0;
	static unsigned char check = 0;
	
	static unsigned int  protMode = 0;
        
  static unsigned lenEx = 0;
	int i,temp;
	if(count == 0)
	{
			if(rec == 0x77) count++;
	}
	else if(count == 1)
	{
			cmdType = rec;
			check += rec;
			count++;
	}
	else if(count == 2)
	{                     
		 decAddr = rec;
		 if(decAddr==0x02)
		 {
			check += rec;
			count++;
		 }
		 else
		 {
			 check=0;
			 count=0;
		 }
	}
	else if(count == 3)
	{
			addrL = rec;
			check += rec;
			count++;
	}
	else if(count == 4)
	{
			addrH = rec;
			check += rec;
			count++;
	}	
	else if(count == 5)	
	{
			if(cmdType == 0)//read
			{
				lenght = 0;
			}
			else
			{
				lenght = rec;
									
			}
			check += rec;
	    count++;
	    lenEx = rec;
	}
	else if(count < 6+lenght)
	{
			buffer[count - 6] = rec;
			check += rec;
			count++;
	}
	else if(count == 6+lenght)
	{
			ErrorCode=rec;
			check += rec;
			count++;    
	}
	else if(count == 7+lenght)
	{
			check += rec;
			count++;
	}
	else if(count == 8+lenght)
	{
			if(rec == 0xAA)
			{
					//校验
					if(check == 0xff)
					{
							//本机地址
							if(decAddr == gDecAddr)
							{
									switch(cmdType)
								{
									//写指令
									case 0x01:
										//通常地址
										if(addrH < 0x80)
										{
											/*gProtBuffer[0]=buffer[0]+(buffer[1]<<8);
                      (*gProtWrite[0])(0,0,gProtBuffer[0]);*/
                      temp = GetLineAddrByAddr(addrH,addrL);
											for(i = 0;i<lenght / 2;i++)
											{
												 gProtBuffer[temp + i] = buffer[i*2] + ((unsigned int)buffer[i*2 +1]<<8);
												 gProtWriteFlag[temp + i] = 1;
											}
											if(protMode == 0)
											{
												for(i = 0; i < gAddrLenSum;i++)
												{
													if(gProtWriteFlag[i] != 0)
													{
														(*gProtWrite[i])(GetAddrHByLineAddr(i),GetAddrLByLineAddr(i),gProtBuffer[i]);
                            // SendWriteAck(addrH,addrL);
													}
													gProtWriteFlag[i] = 0;
												}
											}
										}
										else if(addrH == 0x80 && addrL == 0x00)
										{
												protMode = buffer[0] + (unsigned int)buffer[1]* 256;
										}
										else if(addrH == 0x80 && addrL == 0x01 && buffer[0] != 0x00)
										{
												for(i = 0; i < gAddrLenSum;i++)
												{
													if(gProtWriteFlag[i] == 1)
													{
														(*gProtWrite[i])(GetAddrHByLineAddr(i),GetAddrLByLineAddr(i),gProtBuffer[i]);
													}
													gProtWriteFlag[i] = 0;
												}
										}
										break;
									//读指令
									case 0x00:
										SendReadAck(addrH,addrL,lenEx);
										break;
									//ping
									case 0x02:
										//USART1_SendEx(0x03);
										break;
									default :
										//USART1_SendEx(0xaa);
										break;
								}
							}
					}
					else
					{
						USART1_SendDataString(&check,1);
					}
			}
			check = 0;
			count = 0;
	}
}
		
	
	/******************* (C) COPYRIGHT 2013 SIAT *****END OF FILE****/
