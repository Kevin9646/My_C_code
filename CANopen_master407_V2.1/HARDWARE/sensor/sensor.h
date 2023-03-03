#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h" 
#include "main.h"
 

/*下面的方式是通过直接操作库函数方式读取IO*/
//#define KEY0 		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4) //PE4
//#define KEY1 		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)	//PE3 
//#define KEY2 		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2) //PE2
//#define WK_UP 	GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)	//PA0


/*下面方式是通过位带操作方式读取IO*/


/* 升降电机原点极限信号*/
#define SmallLiftHome                   PDin(4)
#define SmallLiftLimit_Up               PDin(2)
#define SmallLiftLimit_Down             PDin(0)
#define BigLiftHome                     PAin(6)
#define BigLiftLimit_Up                 PBin(3)
#define BigLiftLimit_Down               PDin(6)


/* 步进电机原点极限信号 */
#define SmallForkHome 		  						PBin(0)   			
#define SmallForkLimit_L 								PEin(7)		 		

#define BigForkHome 		  							PBin(11)   	
#define BigForkLimit_L 									PBin(13)		 
#define BigForkLimit_R 									PBin(15)		

#define RotateHome 		  								PEin(11)


/* 放箱时传感器检测料箱有无，判断大叉是否工作*/
#define BigForkWorkDetect_Place_Left 		PEin(13)	
#define BigForkWorkDetect_Place_Right 	PEin(15)	


/* 大叉检测是否有料箱，判断是否需要伸出大叉 */
#define BigForkWorkDetect_Left 		  		PAin(5)																	/* 大叉工作检测左 */
#define BigForkWorkDetect_Right 		  	PCin(5)																	/* 大叉工作检测右 */	

#define BigForkWorkDetectFirstFloor_Left  PAin(4)
#define BigForkWorkDetectFirstFloor_Right  PBin(14)


/* 判断大叉是否允许返回 */
#define BigForkReturnIsORNo							PBin(1)																		/* 大叉缩回 */

/* 判断大叉是否可以伸出*/
#define BigForkExtend_Left_Outside     PEin(12)
#define BigForkExtend_Left_Inside      PEin(8)
#define BigForkExtendDetect_Left       (PEin(12) && PEin(8))
#define BigForkExtend_Right_Outside    PEin(10)
#define BigForkExtend_Right_Inside     PBin(12)
#define BigForkExtendDetect_Right    (PEin(10) && PEin(12))

/* 判断小叉是否可以伸出-----旋转处于原点，然后左侧右侧*/
#define SmallForkExtend_Left           PDin(1)
#define SmallForkExtend_Right          PDin(5)

#define SmallForkExtendDetect          (PDin(1) && PDin(5))


/*料箱嵌套检测*/
#define ToteInrightPosition_Front           PEin(9)
#define ToteInrightPosition_Back            PDin(7)
#define ToteInrightPosition_Left            PDin(11)
#define ToteInrightPosition_Right           PDin(3)


#define Angle_Home        2047     //   2053-102  2047-103  2048-104--2049  2053-105
#define Angle_Left        1025     //   1020-102  1025-103  1026-104--1027  1027-105
#define Angle_Right       3065     //   3078-102  3065-103  3073-104--3070  3076-105

#define Angle_Window      2



/* 检测小叉是否叉了料箱 */
//#define SmallForkToteBin								PCin(5)																		/* 小叉料箱 */

///* 大小叉碰撞 */
//#define BigForkNearSmallFork						PBin(1)																		/* 大小叉碰撞 */


//struct SteptMotor_MoveState
//{
//	u8 BigFork_Home;									/* 大叉原点 */
//	u8 BigFork_Extent_L;							/* 大叉左伸缩 */
//	u8 BigFork_Extent_R;							/* 大叉右伸缩 */
//	u8 SmallFork_Home;								/* 小叉原点 */
//	u8 SmallFork_Extent;							/* 小叉右伸缩 */	
//	u8 Rotate_Home;										/* 旋转原点 */	
//	u8 Rotate_L;											/* 左旋转 */
//	u8 Rotate_R;											/* 右旋转 */
//};

//extern struct SteptMotor_MoveState	SteptMotor_WorkState;
















void Sensor_Init(void);										//IO初始化
u8 Sensor_Scan(void);  										//按键扫描函数	
void	ScanBigForkNearSmallFork(void);			/* 大小叉接近扫描 */
u8	BigForkShrinkAllow(void);							/* 大叉缩回询问 */
u8 SmallForkAllowExtend(void);
u8 BigForkAllowExtend(void);



#endif
