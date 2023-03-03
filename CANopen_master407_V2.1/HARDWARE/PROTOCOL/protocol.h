#ifndef __PROTOCOL_H
#define __PROTOCOL_H

/**
  ******************************************************************************
  * @file USER/protocol.h 
  * @author  ZNFS Application Team
  * @version  V0.0.1
  * @date  09/19/2013
  * @brief  Protocol file.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2013 SIAT</center></h2>
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/




#include "main.h"
/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/////////////////////////////////////////////////////
///函数名:ProtRecByte
///用于串口接收中断调用
///参数：
///rec	接收到的数据
///返回：
///无
/////////////////////////////////////////////////////
void ProtRecByte(unsigned char rec);
void ProtRecByte3(unsigned char rec);
extern u16 angle;
/////////////////////////////////////////////////////
///函数名:SetDecAddr
///设置本机地址
///参数：
///decAddr 本机地址
///返回：
///无
/////////////////////////////////////////////////////
void SetDecAddr(unsigned char decAddr);
/////////////////////////////////////////////////////
///函数名:SetAddrLenght
///用于设置指定H地址内寄存器的个数，只有在DoInitProt()之前调用有效
///不支持实时修改
///参数：
///addrH	指定的H地址
///len		包含寄存器的数量（int为单位）
///返回：
///无
/////////////////////////////////////////////////////
void SetAddrLenght(unsigned char addrH,unsigned char len);

/////////////////////////////////////////////////////
///函数名:DoInitProt
///初始化协议服务，在串口开始工作前调用。
///不支持多次初始化，初始化前请调用SetAddrLenght()配置地址
///调用完成后可使用SetWriteFunc()和SetReadFunc()设置回调函数
///参数：
///无
///返回：
///无
/////////////////////////////////////////////////////
void DoInitProt(void);

/////////////////////////////////////////////////////
///函数名:SetWriteFunc
///用于设置写回调函数，当指定地址发生写操作时，回调函数将被调用。
///必须在DoInitProt()调用后使用，支持动态修改，新值会覆盖原值
///初始化后默认为DefaultWriteFunc()
///参数：
///addrH		H地址
///addrL		L地址
///func			设置的回调函数，类型请参考DefaultWriteFunc()
///返回：
///无
/////////////////////////////////////////////////////
void SetWriteFunc(unsigned char addrH,unsigned char addrL, void (* func)(unsigned char,unsigned char,unsigned int));

/////////////////////////////////////////////////////
///函数名:SetReadFunc
///用于设置读回调函数，当指定地址发生读操作时，回调函数将被调用。
///必须在DoInitProt()调用后使用，支持动态修改，新值会覆盖原值
///初始化后默认为DefaultReadFunc()
///参数：
///addrH		H地址
///addrL		L地址
///func			设置的回调函数，类型请参考DefaultReadFunc()
///返回：
///无
/////////////////////////////////////////////////////
void SetReadFunc(unsigned char addrH,unsigned char addrL,unsigned int(* func)(int));

/////////////////////////////////////////////////////
///函数名:DefaultWriteFunc
///默认写操作回调函数。
///向默认存储表内对应地址写入数据。
///参数：
///addrH	写入数据的H地址
///addrL	写入数据的L地址
///val		写入数据的内容
///返回：
///无
/////////////////////////////////////////////////////
void DefaultWriteFunc(unsigned char addrH,unsigned char addrL,unsigned int val);

/////////////////////////////////////////////////////
///函数名:DefaultReadFunc
///默认读操作回调函数。
///从默认存储表内对应地址读出数据。
///参数：
///addrH	数据的H地址
///addrL	数据的L地址
///返回：
///读出的数据
/////////////////////////////////////////////////////
unsigned int DefaultReadFunc(unsigned char addrH,unsigned char addrL);
unsigned int DefaultReadLineAddrFunc(int lineAddr);
/****************************************************************/
/*************************参考初始化代码*************************/
/****************************************************************/
/*初始化部分
//首先设置本机地址
SetDecAddr(0x02);	//将地址设为0x02
//初始化所有内存表大小，示例中为0x01 1个，0x02 2个，0x03 4个
SetAddrLenght(0x01,1);
SetAddrLenght(0x02,2);
SetAddrLenght(0x03,4);
//执行DoInitProt()
DoInitProt();
//配置回调函数(函数声明见下)
SetWriteFunc(0x01,0x00,MyWriteFunc);
SetReadFunc(0x03,0x03,MyReadFunc);
//初始化完成

//自定义回调函数:写函数
void MyWriteFunc(unsigned char addrH,unsigned char addrL,unsigned int val)
{
	//根据地址可以进行相关操作，如果只配置了一个地址，则也可直接进行操作
	//比如 PortA = val;
	
	DefaultWriteFunc(addrH,addrL,val);		//函数中调用DefaultWriteFunc()，可利用默认表存储数据
																				//需要的时候，可以使用DefaultReadFunc()读取
}

//自定义回调函数：读函数
unsigned int MyReadFunc(unsigned char addrH,unsigned char addrL)
{
	//根据地址进行相关操作，如果只配置了一个地址，则也可直接进行操作
	//如：return PortA;
	
	//也可先执行部分自定义操作，然后返回默认表中的数据
	// ...
	return DefaultReadFunc(addrH,addrL);
}

//其他注意事项：
//1.所有的存储、读取操作，请使用DefaultWriteFunc(),DefaultReadFunc()进行。
//2.恩...还没想到
*/
#endif /* __PROTOCOL1_H */

/******************* (C) COPYRIGHT 2013 SIAT *****END OF FILE****/
