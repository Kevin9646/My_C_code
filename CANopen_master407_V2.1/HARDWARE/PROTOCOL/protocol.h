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
///������:ProtRecByte
///���ڴ��ڽ����жϵ���
///������
///rec	���յ�������
///���أ�
///��
/////////////////////////////////////////////////////
void ProtRecByte(unsigned char rec);
void ProtRecByte3(unsigned char rec);
extern u16 angle;
/////////////////////////////////////////////////////
///������:SetDecAddr
///���ñ�����ַ
///������
///decAddr ������ַ
///���أ�
///��
/////////////////////////////////////////////////////
void SetDecAddr(unsigned char decAddr);
/////////////////////////////////////////////////////
///������:SetAddrLenght
///��������ָ��H��ַ�ڼĴ����ĸ�����ֻ����DoInitProt()֮ǰ������Ч
///��֧��ʵʱ�޸�
///������
///addrH	ָ����H��ַ
///len		�����Ĵ�����������intΪ��λ��
///���أ�
///��
/////////////////////////////////////////////////////
void SetAddrLenght(unsigned char addrH,unsigned char len);

/////////////////////////////////////////////////////
///������:DoInitProt
///��ʼ��Э������ڴ��ڿ�ʼ����ǰ���á�
///��֧�ֶ�γ�ʼ������ʼ��ǰ�����SetAddrLenght()���õ�ַ
///������ɺ��ʹ��SetWriteFunc()��SetReadFunc()���ûص�����
///������
///��
///���أ�
///��
/////////////////////////////////////////////////////
void DoInitProt(void);

/////////////////////////////////////////////////////
///������:SetWriteFunc
///��������д�ص���������ָ����ַ����д����ʱ���ص������������á�
///������DoInitProt()���ú�ʹ�ã�֧�ֶ�̬�޸ģ���ֵ�Ḳ��ԭֵ
///��ʼ����Ĭ��ΪDefaultWriteFunc()
///������
///addrH		H��ַ
///addrL		L��ַ
///func			���õĻص�������������ο�DefaultWriteFunc()
///���أ�
///��
/////////////////////////////////////////////////////
void SetWriteFunc(unsigned char addrH,unsigned char addrL, void (* func)(unsigned char,unsigned char,unsigned int));

/////////////////////////////////////////////////////
///������:SetReadFunc
///�������ö��ص���������ָ����ַ����������ʱ���ص������������á�
///������DoInitProt()���ú�ʹ�ã�֧�ֶ�̬�޸ģ���ֵ�Ḳ��ԭֵ
///��ʼ����Ĭ��ΪDefaultReadFunc()
///������
///addrH		H��ַ
///addrL		L��ַ
///func			���õĻص�������������ο�DefaultReadFunc()
///���أ�
///��
/////////////////////////////////////////////////////
void SetReadFunc(unsigned char addrH,unsigned char addrL,unsigned int(* func)(int));

/////////////////////////////////////////////////////
///������:DefaultWriteFunc
///Ĭ��д�����ص�������
///��Ĭ�ϴ洢���ڶ�Ӧ��ַд�����ݡ�
///������
///addrH	д�����ݵ�H��ַ
///addrL	д�����ݵ�L��ַ
///val		д�����ݵ�����
///���أ�
///��
/////////////////////////////////////////////////////
void DefaultWriteFunc(unsigned char addrH,unsigned char addrL,unsigned int val);

/////////////////////////////////////////////////////
///������:DefaultReadFunc
///Ĭ�϶������ص�������
///��Ĭ�ϴ洢���ڶ�Ӧ��ַ�������ݡ�
///������
///addrH	���ݵ�H��ַ
///addrL	���ݵ�L��ַ
///���أ�
///����������
/////////////////////////////////////////////////////
unsigned int DefaultReadFunc(unsigned char addrH,unsigned char addrL);
unsigned int DefaultReadLineAddrFunc(int lineAddr);
/****************************************************************/
/*************************�ο���ʼ������*************************/
/****************************************************************/
/*��ʼ������
//�������ñ�����ַ
SetDecAddr(0x02);	//����ַ��Ϊ0x02
//��ʼ�������ڴ���С��ʾ����Ϊ0x01 1����0x02 2����0x03 4��
SetAddrLenght(0x01,1);
SetAddrLenght(0x02,2);
SetAddrLenght(0x03,4);
//ִ��DoInitProt()
DoInitProt();
//���ûص�����(������������)
SetWriteFunc(0x01,0x00,MyWriteFunc);
SetReadFunc(0x03,0x03,MyReadFunc);
//��ʼ�����

//�Զ���ص�����:д����
void MyWriteFunc(unsigned char addrH,unsigned char addrL,unsigned int val)
{
	//���ݵ�ַ���Խ�����ز��������ֻ������һ����ַ����Ҳ��ֱ�ӽ��в���
	//���� PortA = val;
	
	DefaultWriteFunc(addrH,addrL,val);		//�����е���DefaultWriteFunc()��������Ĭ�ϱ�洢����
																				//��Ҫ��ʱ�򣬿���ʹ��DefaultReadFunc()��ȡ
}

//�Զ���ص�������������
unsigned int MyReadFunc(unsigned char addrH,unsigned char addrL)
{
	//���ݵ�ַ������ز��������ֻ������һ����ַ����Ҳ��ֱ�ӽ��в���
	//�磺return PortA;
	
	//Ҳ����ִ�в����Զ��������Ȼ�󷵻�Ĭ�ϱ��е�����
	// ...
	return DefaultReadFunc(addrH,addrL);
}

//����ע�����
//1.���еĴ洢����ȡ��������ʹ��DefaultWriteFunc(),DefaultReadFunc()���С�
//2.��...��û�뵽
*/
#endif /* __PROTOCOL1_H */

/******************* (C) COPYRIGHT 2013 SIAT *****END OF FILE****/
