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


/* �������ԭ�㼫���ź�*/
#define SmallLiftHome                   PDin(4)
#define SmallLiftLimit_Up               PDin(2)
#define SmallLiftLimit_Down             PDin(0)
#define BigLiftHome                     PAin(6)
#define BigLiftLimit_Up                 PBin(3)
#define BigLiftLimit_Down               PDin(6)


/* �������ԭ�㼫���ź� */
#define SmallForkHome 		  						PBin(0)   			
#define SmallForkLimit_L 								PEin(7)		 		

#define BigForkHome 		  							PBin(11)   	
#define BigForkLimit_L 									PBin(13)		 
#define BigForkLimit_R 									PBin(15)		

#define RotateHome 		  								PEin(11)


/* ����ʱ����������������ޣ��жϴ���Ƿ���*/
#define BigForkWorkDetect_Place_Left 		PEin(13)	
#define BigForkWorkDetect_Place_Right 	PEin(15)	


/* ������Ƿ������䣬�ж��Ƿ���Ҫ������ */
#define BigForkWorkDetect_Left 		  		PAin(5)																	/* ��湤������� */
#define BigForkWorkDetect_Right 		  	PCin(5)																	/* ��湤������� */	

#define BigForkWorkDetectFirstFloor_Left  PAin(4)
#define BigForkWorkDetectFirstFloor_Right  PBin(14)


/* �жϴ���Ƿ������� */
#define BigForkReturnIsORNo							PBin(1)																		/* ������� */

/* �жϴ���Ƿ�������*/
#define BigForkExtend_Left_Outside     PEin(12)
#define BigForkExtend_Left_Inside      PEin(8)
#define BigForkExtendDetect_Left       (PEin(12) && PEin(8))
#define BigForkExtend_Right_Outside    PEin(10)
#define BigForkExtend_Right_Inside     PBin(12)
#define BigForkExtendDetect_Right    (PEin(10) && PEin(12))

/* �ж�С���Ƿ�������-----��ת����ԭ�㣬Ȼ������Ҳ�*/
#define SmallForkExtend_Left           PDin(1)
#define SmallForkExtend_Right          PDin(5)

#define SmallForkExtendDetect          (PDin(1) && PDin(5))


/*����Ƕ�׼��*/
#define ToteInrightPosition_Front           PEin(9)
#define ToteInrightPosition_Back            PDin(7)
#define ToteInrightPosition_Left            PDin(11)
#define ToteInrightPosition_Right           PDin(3)


#define Angle_Home        2047     //   2053-102  2047-103  2048-104--2049  2053-105
#define Angle_Left        1025     //   1020-102  1025-103  1026-104--1027  1027-105
#define Angle_Right       3065     //   3078-102  3065-103  3073-104--3070  3076-105

#define Angle_Window      2



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
u8 SmallForkAllowExtend(void);
u8 BigForkAllowExtend(void);



#endif
