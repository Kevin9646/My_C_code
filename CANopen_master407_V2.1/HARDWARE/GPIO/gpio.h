#ifndef __GPIO_H
#define __GPIO_H
#include "main.h"
#include "sys.h"




//�����������ʹ�ܶ���
#define 	SmallFork_Enable				PCout(10)		//  С��ʹ��	


#define 	BigFork_Enable 					PDout(14)		//  ��תʹ��			 
#define 	Rotate_Enable 					PCout(9)		//  ���ʹ��


//#define 	Rotate_Enable 					PDout(14)		//  ��תʹ��			 
//#define 	BigFork_Enable 					PCout(9)		//  ���ʹ��

#define 	SmallFork_DIR 					PCout(11)		//  С�淽��



#define 	Rotate_DIR 						PCout(8)		// 	��淽��	
#define 	BigFork_DIR 							PDout(13)		// 	��ת����	

//#define 	BigFork_DIR 						PCout(8)		// 	��淽��	
//#define 	Rotate_DIR 							PDout(13)		// 	��ת����	

/* RS485ʹ�ܽǶ��� */
#define 	RS485_Control						PDout(10)		/* 485��������		0:RXEN   1:TXEN	 */

/* ���TXEN_A1    RXEN_A0    */
#define 	LOBOT_TXEN 										PAout(1)		// 	���TXEN	
#define 	LOBOT_RXEN 										PAout(0)		// 	���RXEN	



void gpio_Init(void);//��ʼ��		 				    
#endif									/* __GPIO_H */
