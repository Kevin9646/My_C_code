#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h" 
#include "main.h"
 

/*����ķ�ʽ��ͨ��ֱ�Ӳ����⺯����ʽ��ȡIO*/
//#define KEY0 		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4) //PE4
//#define KEY1 		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)	//PE3 
//#define KEY2 		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2) //PE2
//#define WK_UP 	GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)	//PA0


/*���淽ʽ��ͨ��λ��������ʽ��ȡIO*/


/* �������ԭ�㼫���ź� */
#define SmallForkHome 		  						PBin(0)   			
#define SmallForkLimit_L 								PEin(7)		 
//#define SmallForkLimit_R 								PEin(9)		

#define BigForkHome 		  							PBin(11)   	
#define BigForkLimit_L 									PBin(13)		 
#define BigForkLimit_R 									PBin(15)		

#define RotateHome 		  								PEin(11)   	
#define RotateLimit_L 									PEin(13)	
#define RotateLimit_R 								 	PEin(15)	




/* ������Ƿ������䣬�ж��Ƿ���Ҫ������ */
#define BigForkWorkDetect_Left 		  		PDin(9)																	/* ԭ������� */
#define BigForkWorkDetect_Right 		  	PCin(5)																	/* ��湤�� */					

/* �жϴ���Ƿ������� */
#define BigForkReturnIsORNo							PBin(1)																		/* ������� */

/* �жϴ���Ƿ�������*/
#define BigForkExtend_Left_Outside     PEin(12)
#define BigForkExtend_Left_Inside      PEin(8)
#define BigForkExtend_Right_Outside    PEin(10)
#define BigForkExtend_Right_Inside     PBin(12)

/* �ж�С���Ƿ�������-----��ת����ԭ�㣬Ȼ������Ҳ�*/
#define SmallForkExtend_Left           PDin(1)
#define SmallForkExtend_Right          PDin(5)


/*����Ƕ�׼��*/
#define ToteInrightPosition_Front           PEin(6)
#define ToteInrightPosition_Back            PDin(7)
#define ToteInrightPosition_Left            PDin(11)
#define ToteInrightPosition_Right           PDin(3)



/* ���С���Ƿ�������� */
//#define SmallForkToteBin								PCin(5)																		/* С������ */

///* ��С����ײ */
//#define BigForkNearSmallFork						PBin(1)																		/* ��С����ײ */


//struct SteptMotor_MoveState
//{
//	u8 BigFork_Home;									/* ���ԭ�� */
//	u8 BigFork_Extent_L;							/* ��������� */
//	u8 BigFork_Extent_R;							/* ��������� */
//	u8 SmallFork_Home;								/* С��ԭ�� */
//	u8 SmallFork_Extent;							/* С�������� */	
//	u8 Rotate_Home;										/* ��תԭ�� */	
//	u8 Rotate_L;											/* ����ת */
//	u8 Rotate_R;											/* ����ת */
//};

//extern struct SteptMotor_MoveState	SteptMotor_WorkState;
















void Sensor_Init(void);										//IO��ʼ��
u8 Sensor_Scan(void);  										//����ɨ�躯��	
void	ScanBigForkNearSmallFork(void);			/* ��С��ӽ�ɨ�� */
u8	BigForkShrinkAllow(void);							/* �������ѯ�� */



#endif
