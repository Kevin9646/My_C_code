#ifndef __GPIO_H
#define __GPIO_H
#include "main.h"
#include "sys.h"




//步进电机方向使能定义
#define 	SmallFork_Enable				PCout(10)		//  小叉使能	


#define 	BigFork_Enable 					PDout(14)		//  旋转使能			 
#define 	Rotate_Enable 					PCout(9)		//  大叉使能


//#define 	Rotate_Enable 					PDout(14)		//  旋转使能			 
//#define 	BigFork_Enable 					PCout(9)		//  大叉使能

#define 	SmallFork_DIR 					PCout(11)		//  小叉方向



#define 	Rotate_DIR 						PCout(8)		// 	大叉方向	
#define 	BigFork_DIR 							PDout(13)		// 	旋转方向	

//#define 	BigFork_DIR 						PCout(8)		// 	大叉方向	
//#define 	Rotate_DIR 							PDout(13)		// 	旋转方向	

/* RS485使能角定义 */
#define 	RS485_Control						PDout(10)		/* 485控制引脚		0:RXEN   1:TXEN	 */

/* 舵机TXEN_A1    RXEN_A0    */
#define 	LOBOT_TXEN 										PAout(1)		// 	舵机TXEN	
#define 	LOBOT_RXEN 										PAout(0)		// 	舵机RXEN	



void gpio_Init(void);//初始化		 				    
#endif									/* __GPIO_H */
