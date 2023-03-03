#include "sensor.h"
#include "delay.h" 


//struct SteptMotor_MoveState	SteptMotor_WorkState;

u8 BigForkNearSmallForkFlag	= 0;

u8 SaveBigForkSpeed = 0;
u8 SaveSmallForkSpeed = 0;

//传感器初始化函数
void Sensor_Init(void)
{
	
	GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_GPIOE, ENABLE);//使能时钟
 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_13|GPIO_Pin_15; //
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//浮空
  GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化
	
	 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_3|GPIO_Pin_5|GPIO_Pin_7|GPIO_Pin_0|GPIO_Pin_11|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;//WK_UP对应引脚
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//浮空
  GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_11|GPIO_Pin_10|GPIO_Pin_9;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//浮空
  GPIO_Init(GPIOD, &GPIO_InitStructure);//初始化	
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//浮空
  GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化		
} 


u8 Sensor_Scan(void)
{	 
	/* ---------------------------------------------------------------------小叉伸缩			start */
	
	if(SmallForkHome == 0)																								/* 小叉回原信号触发 */
	{	
		if(SmallForkHome == 0)																							/* 二次确认小叉回原信号触发 */
		{	
			if(RobotState.ReturnHome_State[1] == 0xff)												/* 回原状态被标记 */
			{
				RobotFinishFlag.ReturnHome_State_finish[1] = 0xff;							/* 找到原点，关闭电机，设置完成标志，清除状态标志 */
				CloseSteptMotor(SmallFork_Num);
				RobotState.ReturnHome_State[1] = 0;
			}
			
			// 小叉缩回检测到传感器
			if(RobotState.SmallFork_Shrink  == 0xff)
			{
				CloseSteptMotor(SmallFork_Num);
				RobotState.SmallFork_Shrink = 0;
				RobotFinishFlag.SmallFork_Shrink_finish = 0xff;			
			}	
		}
	}
	
	if (SmallForkLimit_L == 0)
	{
		if (SmallForkLimit_L == 0)
		{
			// 小叉伸出检测到传感器
			if(RobotState.SmallFork_Extend  == 0xff) 
			{
				CloseSteptMotor(SmallFork_Num);
				RobotState.SmallFork_Extend  = 0;
				RobotFinishFlag.SmallFork_Extend_finish  = 0xff;			
			}
		}
	}
	/* -------------------------------------------------------------------小叉伸缩			end */	
	
	
	
	/* 大叉伸缩 */
	if(BigForkHome == 0)																											/* 大叉回原信号触发 */
	{				
		if(BigForkHome == 0)																										/* 大叉回原信号触发二次验证 */
		{	
			if(RobotState.ReturnHome_State[0] == 0xff)														/* 处于回原状态 */
			{
				if(RobotState.steptBigForkReturnHomeDir == ReturnHome_Left)
				{					
					if(RobotState.steptBigForkReturnChangeDir == 0xff)
					{
						BigForkReturnHome(0) = -1;
						RobotState.steptBigForkReturnChangeDir = 0;
					}
				}
				if(RobotState.steptBigForkReturnHomeDir == ReturnHome_Right)
				{
					
					CloseSteptMotor(BigFork_Num);
					RobotState.ReturnHome_State[0] = 0;
					RobotState.BigForkBlindFindHome = 0;
					RobotState.steptBigForkReturnHomeDir = 0;
					RobotFinishFlag.ReturnHome_State_finish[0] = 0xff;									/* 设置完成标志，清除状态标志 */
				}
			}
			if(RobotState.BigFork_Shrink_left  == 0xff) 
			{
				CloseSteptMotor(BigFork_Num);
				RobotState.BigFork_Shrink_left = 0;
				RobotFinishFlag.BigFork_Shrink_left_finish = 0xff;			
			}
			if(RobotState.BigFork_Shrink_right  == 0xff)
			{
				CloseSteptMotor(BigFork_Num);
				RobotState.BigFork_Shrink_right = 0;
				RobotFinishFlag.BigFork_Shrink_right_finish = 0xff;			
			}
		}
	}

	if(BigForkLimit_L == 0)
	{		
		if(BigForkLimit_L == 0)																										/* 二次确认左极限触发 */
		{
			if(RobotState.BigForkBlindFindHome == 0xff)
			{
				RobotState.BigForkBlindFindHome = 0;																	/* 找到左极限后就变成从左向右找原点了，可清除盲找信号 */
				CloseSteptMotor(BigFork_Num);
				BigFork_SteptMotor_Start(BigFork_ReturnHome.ReturnHome_Position,BigFork_ReturnHome.ReturnHome_Speed,BigFork_ReturnHome.ReturnHome_Accel,BigFork_ReturnHome.ReturnHome_Decel,Positive);	/* 重启向左运动继续寻找原点 */			
				RobotState.steptBigForkReturnHomeDir = ReturnHome_Right;								/* 标记大叉回原反向，碰到右极限，向左找原点 */
				RobotState.steptBigForkReturnChangeDir = 0xff;			
			}			
			if(RobotState.BigFork_Extend_left  == 0xff) 													
			{
				CloseSteptMotor(BigFork_Num);
				RobotState.BigFork_Extend_left  = 0;
				RobotFinishFlag.BigFork_Extend_left_finish  = 0xff;			
			}
		}
	}
	
	if(BigForkLimit_R == 0)
	{
		if(RobotState.BigFork_Extend_right  == 0xff) 
		{
			CloseSteptMotor(BigFork_Num);
			RobotState.BigFork_Extend_right  = 0;
			RobotFinishFlag.BigFork_Extend_right_finish  = 0xff;			
		}	
	}
		
	
	/* 旋转 */
	if(RotateHome == 0)
	{		
		if(RotateHome == 0)
		{		
			if(RobotState.ReturnHome_State[2] == 0xff)
			{
				
				if(RobotState.steptRotateReturnHomeDir == ReturnHome_Left)
				{					
					if(RobotState.steptRotatekReturnChangeDir == 0xff)
					{
						RotateReturnHome(0) = -1;
						RobotState.steptRotatekReturnChangeDir = 0;
					}
				}
				if(RobotState.steptRotateReturnHomeDir == ReturnHome_Right)
				{
					RobotFinishFlag.ReturnHome_State_finish[2] = 0xff;
					CloseSteptMotor(Rotate_Num);
					RobotState.ReturnHome_State[2] = 0;
					RobotState.RotateBlindFindHome = 0;
					RobotState.steptRotateReturnHomeDir = 0;
				}
			}
			if(RobotState.Rotate_back_left  == 0xff) 
			{	
				CloseSteptMotor(Rotate_Num);				
				RobotState.Rotate_back_left = 0;
				RobotFinishFlag.Rotate_back_left_finish = 0xff;				
			}	
			if((RobotState.Rotate_back_right  == 0xff) && (RotateMotorPoistion(0) == -1)) 
			{
				RotateMotorPoistion(0) = 0;
				CloseSteptMotor(Rotate_Num);		
				RobotState.Rotate_back_right = 0;
				RobotFinishFlag.Rotate_back_right_finish = 0xff;					
								
			}	
		}
	}

	if(RotateLimit_R == 0)
	{
		if(RotateLimit_R == 0)
		{
			if(RobotState.RotateBlindFindHome == 0xff)
			{
				RobotState.RotateBlindFindHome = 0;																	/* 找到右极限后就变成从右向左找原点了，可清除盲找信号 */
				CloseSteptMotor(Rotate_Num);
				Rotate_SteptMotor_Start(Rotate_Return.ReturnHome_Position,Rotate_Return.ReturnHome_Speed,Rotate_Return.ReturnHome_Accel,Rotate_Return.ReturnHome_Decel,negative);	/* 重启向左运动继续寻找原点 */
				RobotState.steptRotateReturnHomeDir = ReturnHome_Left;								/* 标记旋转回原反向，碰到右极限，向左找原点 */
				RobotState.steptRotatekReturnChangeDir = 0xff;	
			}
			
			if(RobotState.Rotate_out_right  == 0xff) 
			{
				CloseSteptMotor(Rotate_Num);
				RobotState.Rotate_out_right  = 0;
				RobotFinishFlag.Rotate_out_right_finish  = 0xff;	
			}
			if((RobotState.Rotate_behind_back_right == 0xff) && (RotateMotorPoistion(0) == -1))
			{
				RotateMotorPoistion(0) = 0;
				CloseSteptMotor(Rotate_Num);
				RobotState.Rotate_behind_back_right = 0;
				RobotFinishFlag.Rotate_back_behind_right_finish = 0xff;		
			}
		}
	}
	
	if(RotateLimit_L == 0)
	{
		if (RotateLimit_L == 0)
		{
			if(RobotState.Rotate_out_left  == 0xff) 
			{
				CloseSteptMotor(Rotate_Num);
				RobotState.Rotate_out_left  = 0;
				RobotFinishFlag.Rotate_out_left_finish  = 0xff;
			}
			if((RobotState.Rotate_behind_back_left == 0xff) && (RotateMotorPoistion(0) == -1))
			{
				RotateMotorPoistion(0) = 0;
				CloseSteptMotor(Rotate_Num);
				RobotState.Rotate_behind_back_left = 0;
				RobotFinishFlag.Rotate_back_behind_left_finish = 0xff;
			}
		}
	}
 	return 0;
}


//void	ScanBigForkNearSmallFork(void)
//{
//	if(BigForkNearSmallFork == 0)
//	{
//			if(BigForkNearSmallFork == 0)
//			{
//				
//				if(BigForkNearSmallForkFlag == 0)
//				{
//						controlWord_6040[0] = 0x06;
//						controlWord_6040[1] = 0x06;	
//						BigForkNearSmallForkFlag = 0xff;
//				}
//			}
//			else
//			{
//				BigForkNearSmallForkFlag = 0;
//			}
//	}
//	else
//	{
//		BigForkNearSmallForkFlag = 0;
//	}
//	
//}



u8	BigForkShrinkAllow(void)
{
	if(BigForkReturnIsORNo == 0)
	{
			if(BigForkReturnIsORNo == 0)
			{
				return 1;
			}
			else
			{
				return 0;
			}
	}
	else
	{
		return 0;
	}
}










