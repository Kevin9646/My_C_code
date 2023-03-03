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


/* 步进电机原点极限信号 */
#define SmallForkHome 		  						PBin(0)   			
#define SmallForkLimit_L 								PEin(7)		 
//#define SmallForkLimit_R 								PEin(9)		

#define BigForkHome 		  							PBin(11)   	
#define BigForkLimit_L 									PBin(13)		 
#define BigForkLimit_R 									PBin(15)		

#define RotateHome 		  								PEin(11)   	
#define RotateLimit_L 									PEin(13)	
#define RotateLimit_R 								 	PEin(15)	




/* 大叉检测是否有料箱，判断是否需要伸出大叉 */
#define BigForkWorkDetect_Left 		  		PDin(9)																	/* 原大叉料箱 */
#define BigForkWorkDetect_Right 		  	PCin(5)																	/* 大叉工作 */					

/* 判断大叉是否允许返回 */
#define BigForkReturnIsORNo							PBin(1)																		/* 大叉缩回 */

/* 判断大叉是否可以伸出*/
#define BigForkExtend_Left_Outside     PEin(12)
#define BigForkExtend_Left_Inside      PEin(8)
#define BigForkExtend_Right_Outside    PEin(10)
#define BigForkExtend_Right_Inside     PBin(12)

/* 判断小叉是否可以伸出-----旋转处于原点，然后左侧右侧*/
#define SmallForkExtend_Left           PDin(1)
#define SmallForkExtend_Right          PDin(5)


/*料箱嵌套检测*/
#define ToteInrightPosition_Front           PEin(6)
#define ToteInrightPosition_Back            PDin(7)
#define ToteInrightPosition_Left            PDin(11)
#define ToteInrightPosition_Right           PDin(3)



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



#endif
