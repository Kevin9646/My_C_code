#include "motorContral.h"
#include "math.h"

struct StepMotorInitDef  StepSmallFork;					/* 步进电机小叉参数定义 */
struct StepMotorInitDef  StepBigFork;						/* 步进电机大叉参数定义 */
struct StepMotorInitDef  StepRotateFork;				/* 步进电机旋转参数定义 */
struct StepReturnHome	BigFork_ReturnHome,SmallFork_Return,Rotate_Return;					/* 回原参数 */

/* 	
	stepMotor_Calculate：运动参数计算
	step：步数 
	accel：加速度 			0.5表示0.5圈/s2即第一秒0.5圈/s,第二秒1圈/s
	decel：减速度 
	speed：最大速度
*/

struct StepMotorInitDef  stepMotor_Calculate(u32 step,float accel,float decel, float speed)
{
	
	float i;
	float stepnCount = 0;										/* 第n步的计数值 */
	struct StepMotorInitDef Parameter;
	memset(&Parameter,0,sizeof(Parameter));
	
	Parameter.allStep = step;
	Parameter.max_speed_count = CountSpeedChange(speed);					/* 加速到最大速度的计数值 */
	Parameter.accelTime = (speed -  startSpeed) / accel;		/* 计算加速时间 */
	Parameter.countRate_Acc = (startSpeed_count - Parameter.max_speed_count) / Parameter.accelTime;   /* 计数器的变化率计算 */
	
	/* 计算加速步数 */
	for(i=0;i<(int)(Parameter.accelTime*1000);i++)
	{
		stepnCount = FunLine(startSpeed_count,Parameter.countRate_Acc,i/1000);
		Parameter.accel_step = Parameter.accel_step + (u16)(accel_T/((stepnCount/ft)*1000)); 
	}
	
	Parameter.decelTime = (speed - startSpeed)  / decel;		/* 计算减速时间 */
	Parameter.countRate_Dec = (startSpeed_count - Parameter.max_speed_count) / Parameter.decelTime;   /* 计数器的变化率计算 */
	
		/* 计算减速步数 */
	for(i=0;i<(int)(Parameter.decelTime*1000);i++)
	{
		stepnCount = FunLine_dec(Parameter.max_speed_count ,Parameter.countRate_Dec,i/1000);
		Parameter.decel_step = Parameter.decel_step + (u16)(accel_T/((stepnCount/ft)*1000)); 
	}
	
	if((Parameter.accel_step + Parameter.decel_step) > Parameter.allStep)		/* 判断是否能够加速到最大速度 */
	{
		Parameter.acc_dec_t = (speed - startSpeed) / (accel + decel);
//		/* 计算该情况下最大速度计速值 */
		Parameter.max_speed_count = FunLine(startSpeed_count,Parameter.countRate_Acc,Parameter.acc_dec_t);
		Parameter.decelTime = (SpeedCountChange(Parameter.max_speed_count) - startSpeed) / decel;
		Parameter.accelTime = Parameter.acc_dec_t;		
		
		Parameter.moveMode = acc_dec;
	}
	else
	{
		Parameter.constant_step = Parameter.allStep - (Parameter.accel_step + Parameter.decel_step);									/* 计算匀速步数 */
		Parameter.constantTime = Parameter.constant_step * (((float)(Parameter.max_speed_count))/ft) ;			/* 匀速时间 */
		Parameter.moveMode = acc_constant_dec;
	}

	/* 给软件定时器赋初值 */
	return Parameter;
}

/*

大叉定时器

timePara: 0 3 6  分别为大叉、小叉、旋转
TIM_NUM:	TIM_BigFork/TIM_SmallFork/TIM_Rotate
*/
/* motorContral(&StepSmallFork,SmallFork_No,TIM_SmallFork); */
void SteptMotor_SoferTime_Big(u8 timePara, TIM_TypeDef *TIM_NUM)
{
	u16 tt =0;
	
	if(StepBigFork.moveMode == acc_dec)					/* 无法达到最高时就减速 */
	{
		if((StepBigFork.motorState == UP) && (BigForkSpeedUp(1) == -1))	/* 加速完成标志 */
		{
			if(StepBigFork.TIMCount_Acc <= ((int)(StepBigFork.accelTime*1000)))
			{
				tt = FunLine(startSpeed_count,StepBigFork.countRate_Acc,StepBigFork.TIMCount_Acc/1000);
				TIM_NUM->ARR = tt;
				
				TIM_NUM->CCR1 = tt/2;																	/* 高级定时器为CCR2 */
	
				StepBigFork.TIMCount_Acc = StepBigFork.TIMCount_Acc + 1;
				BigForkSpeedUp(1) = 0;						/* 加速完成标志 */
				BigForkSpeedUp(3) = 0;						/* 加速计数标志 */
			}
			else 
			{
				StepBigFork.TIMCount_Acc = 0;
				BigForkSpeedUp(0) = 0;									/* 加速使能标志 */
				StepBigFork.motorState = DOWN;			
				BigForkSpeedDown(0) = -1;									/* 减速使能标志 */
			}
		}
		
		if((StepBigFork.motorState == DOWN) && (BigForkSpeedDown(1) == -1))/* 减速完成标志 */	
		{
			if(StepBigFork.TIMCount_Dec <= ((int)(StepBigFork.decelTime*1000)))
			{
				tt = FunLine_dec(StepBigFork.max_speed_count,StepBigFork.countRate_Dec,StepBigFork.TIMCount_Dec/1000);
				TIM_NUM->ARR = tt;

					TIM_NUM->CCR1 = tt/2;


				StepBigFork.TIMCount_Dec = StepBigFork.TIMCount_Dec + 1;
				BigForkSpeedDown(1) = 0;					/* 减速完成标志 */
				BigForkSpeedDown(3) = 0;					/* 减速计数标志 */
			}
			else 
			{
				
				StepBigFork.motorState = WAIT;
				StepBigFork.TIMCount_Dec = 0;
				BigForkSpeedDown(0) = 0;					/* 减速使能标志 */
				memset(&StepBigFork,0,sizeof(StepBigFork));
			}
		}	
	}

  if(StepBigFork.moveMode == acc_constant_dec)																		/* 加速匀速和减速  */
	{
		if((StepBigFork.motorState == UP) && (BigForkSpeedUp(1) == -1))	/* 加速完成标志 */
		{
			if(StepBigFork.TIMCount_Acc <= ((int)(StepBigFork.accelTime*1000)))
			{
				tt = FunLine(startSpeed_count,StepBigFork.countRate_Acc,StepBigFork.TIMCount_Acc/1000);
				TIM_NUM->ARR = tt;

					TIM_NUM->CCR1 = tt/2;

				StepBigFork.TIMCount_Acc = StepBigFork.TIMCount_Acc + 1;
				BigForkSpeedUp(1) = 0;						/* 加速完成标志 */
				BigForkSpeedUp(3) = 0;						/* 加速计数标志 */
			}
			else 
			{
				StepBigFork.TIMCount_Acc = 0;
				BigForkSpeedUp(0) = 0;									/* 加速使能标志 */
				StepBigFork.motorState = CONSTANT;			
				BigForkSpeedCst(0) = -1;									/* 匀速使能标志 */
			}
		}
		
		if((StepBigFork.motorState == CONSTANT) && (BigForkSpeedCst(1) == -1))		/* 匀速完成标志 */
		{
			BigForkSpeedCst(0) = 0;												/* 匀速使能标志 */
			BigForkSpeedDown(0) = -1;												/* 减速使能标志 */		
			StepBigFork.motorState = DOWN;
		}
		
		if((StepBigFork.motorState == DOWN) && (BigForkSpeedDown(1) == -1))/* 减速完成标志 */	
		{
			if(StepBigFork.TIMCount_Dec <= ((int)(StepBigFork.decelTime*1000)))
			{
				tt = FunLine_dec(StepBigFork.max_speed_count ,StepBigFork.countRate_Dec,StepBigFork.TIMCount_Dec/1000);
				TIM_NUM->ARR = tt;

					TIM_NUM->CCR1 = tt/2;
				

				StepBigFork.TIMCount_Dec = StepBigFork.TIMCount_Dec + 1;
				BigForkSpeedDown(1) = 0;					/* 减速完成标志 */
				BigForkSpeedDown(3) = 0;					/* 减速计数标志 */
			}
			else 
			{
				StepBigFork.motorState = WAIT;
				StepBigFork.TIMCount_Dec = 0;
				BigForkSpeedDown(0) = 0;					/* 减速使能标志 */
	
				memset(&StepBigFork,0,sizeof(StepBigFork));
			}
		}	
	}
}


/* 小叉旋转定时器 */

void SteptMotor_SoferTime_Small(u8 timePara, TIM_TypeDef *TIM_NUM)
{
	u16 tt =0;
	
	if(StepSmallFork.moveMode == acc_dec)					/* 无法达到最高时就减速 */
	{
		if((StepSmallFork.motorState == UP) && (timera[timePara+0][1] == -1))	/* 加速完成标志 */
		{
			if(StepSmallFork.TIMCount_Acc <= ((int)(StepSmallFork.accelTime*1000)))
			{
				tt = FunLine(startSpeed_count,StepSmallFork.countRate_Acc,StepSmallFork.TIMCount_Acc/1000);
				TIM_NUM->ARR = tt;
				if(timePara  == Rotate_Num)
				{
					TIM_NUM->CCR2 = tt/2;																	/* 高级定时器为CCR2 */
				}
				else
				{
					TIM_NUM->CCR1 = tt/2;																	/* 通用定时器为CCR1 */
				}
				
				StepSmallFork.TIMCount_Acc ++;
				timera[timePara+0][1] = 0;						/* 加速完成标志 */
				timera[timePara+0][3] = 0;						/* 加速计数标志 */
			}
			else 
			{
				StepSmallFork.TIMCount_Acc = 0;
				timera[timePara+0][0] = 0;									/* 加速使能标志 */
				StepSmallFork.motorState = DOWN;			
				timera[timePara+2][0] = -1;									/* 减速使能标志 */
			}
		}
		
		if((StepSmallFork.motorState == DOWN) && (timera[timePara+2][1] == -1))/* 减速完成标志 */	
		{
			if(StepSmallFork.TIMCount_Dec <= ((int)(StepSmallFork.decelTime*1000)))
			{
				tt = FunLine_dec(StepSmallFork.max_speed_count,StepSmallFork.countRate_Dec,StepSmallFork.TIMCount_Dec/1000);
				TIM_NUM->ARR = tt;
				if(timePara  == Rotate_Num)
				{
					TIM_NUM->CCR2 = tt/2;
				}
				else
				{
					TIM_NUM->CCR1 = tt/2;
				}
				StepSmallFork.TIMCount_Dec ++;
				timera[timePara+2][1] = 0;					/* 减速完成标志 */
				timera[timePara+2][3] = 0;					/* 减速计数标志 */
			}
			else 
			{
				StepSmallFork.motorState = WAIT;
				StepSmallFork.TIMCount_Dec = 0;
				timera[timePara+2][0] = 0;					/* 减速使能标志 */
				memset(&StepSmallFork,0,sizeof(StepSmallFork));
			}
		}	
	}
	
	
	
  if(StepSmallFork.moveMode == acc_constant_dec)																		/* 加速匀速和减速  */
	{
		if((StepSmallFork.motorState == UP) && (timera[timePara+0][1] == -1))	/* 加速完成标志 */
		{
			if(StepSmallFork.TIMCount_Acc <= ((int)(StepSmallFork.accelTime*1000)))
			{
				tt = FunLine(startSpeed_count,StepSmallFork.countRate_Acc,StepSmallFork.TIMCount_Acc/1000);
				TIM_NUM->ARR = tt;
				if(timePara  == Rotate_Num)
				{
					TIM_NUM->CCR2 = tt/2;
				}
				else
				{
					TIM_NUM->CCR1 = tt/2;
				}
				StepSmallFork.TIMCount_Acc ++;
				timera[timePara+0][1] = 0;						/* 加速完成标志 */
				timera[timePara+0][3] = 0;						/* 加速计数标志 */
			}
			else 
			{
				StepSmallFork.TIMCount_Acc = 0;
				timera[timePara+0][0] = 0;									/* 加速使能标志 */
				StepSmallFork.motorState = CONSTANT;			
				timera[timePara+1][0] = -1;									/* 匀速使能标志 */
			}
		}
		
		if((StepSmallFork.motorState == CONSTANT) && (timera[timePara+1][1] == -1))		/* 匀速完成标志 */
		{
			timera[timePara+1][0] = 0;												/* 匀速使能标志 */
			timera[timePara+2][0] = -1;												/* 减速使能标志 */		
			StepSmallFork.motorState = DOWN;
		}
		
		if((StepSmallFork.motorState == DOWN) && (timera[timePara+2][1] == -1))/* 减速完成标志 */	
		{
			if(StepSmallFork.TIMCount_Dec <= ((int)(StepSmallFork.decelTime*1000)))
			{
				tt = FunLine_dec(StepSmallFork.max_speed_count ,StepSmallFork.countRate_Dec,StepSmallFork.TIMCount_Dec/1000);
				TIM_NUM->ARR = tt;
				if(timePara  == Rotate_Num)
				{
					TIM_NUM->CCR2 = tt/2;
				}
				else
				{
					TIM_NUM->CCR1 = tt/2;
				}
				StepSmallFork.TIMCount_Dec ++;
				timera[timePara+2][1] = 0;					/* 减速完成标志 */
				timera[timePara+2][3] = 0;					/* 减速计数标志 */
			}
			else 
			{
				StepSmallFork.motorState = WAIT;	
				StepSmallFork.TIMCount_Dec = 0;
				timera[timePara+2][0] = 0;					/* 减速使能标志 */			
				memset(&StepSmallFork,0,sizeof(StepSmallFork));
				
			}
		}	
	}
}

/* 旋转软件定时器 */

void SteptMotor_SoferTime_Rotate(u8 timePara, TIM_TypeDef *TIM_NUM)
{
	u16 tt =0;
	
	if(StepRotateFork.moveMode == acc_dec)					/* 无法达到最高时就减速 */
	{
		if((StepRotateFork.motorState == UP) && (timera[timePara+0][1] == -1))	/* 加速完成标志 */
		{
			if(StepRotateFork.TIMCount_Acc <= ((int)(StepRotateFork.accelTime*1000)))
			{
				tt = FunLine(startSpeed_count,StepRotateFork.countRate_Acc,StepRotateFork.TIMCount_Acc/1000);
				TIM_NUM->ARR = tt;
				if(timePara  == Rotate_Num)
				{
					TIM_NUM->CCR2 = tt/2;																	/* 高级定时器为CCR2 */
				}
				else
				{
					TIM_NUM->CCR1 = tt/2;																	/* 通用定时器为CCR1 */
				}
				
				StepRotateFork.TIMCount_Acc ++;
				timera[timePara+0][1] = 0;						/* 加速完成标志 */
				timera[timePara+0][3] = 0;						/* 加速计数标志 */
			}
			else 
			{
				StepRotateFork.TIMCount_Acc = 0;
				timera[timePara+0][0] = 0;									/* 加速使能标志 */
				StepRotateFork.motorState = DOWN;			
				timera[timePara+2][0] = -1;									/* 减速使能标志 */
			}
		}
		
		if((StepRotateFork.motorState == DOWN) && (timera[timePara+2][1] == -1))/* 减速完成标志 */	
		{
			if(StepRotateFork.TIMCount_Dec <= ((int)(StepRotateFork.decelTime*1000)))
			{
				tt = FunLine_dec(StepRotateFork.max_speed_count,StepRotateFork.countRate_Dec,StepRotateFork.TIMCount_Dec/1000);
				TIM_NUM->ARR = tt;
				if(timePara  == Rotate_Num)
				{
					TIM_NUM->CCR2 = tt/2;
				}
				else
				{
					TIM_NUM->CCR1 = tt/2;
				}
				StepRotateFork.TIMCount_Dec ++;
				timera[timePara+2][1] = 0;					/* 减速完成标志 */
				timera[timePara+2][3] = 0;					/* 减速计数标志 */
			}
			else 
			{
				StepRotateFork.motorState = WAIT;
				StepRotateFork.TIMCount_Dec = 0;
				timera[timePara+2][0] = 0;					/* 减速使能标志 */	
				memset(&StepRotateFork,0,sizeof(StepRotateFork));
			}
		}	
	}
	
	
  if(StepRotateFork.moveMode == acc_constant_dec)																		/* 加速匀速和减速  */
	{
		if((StepRotateFork.motorState == UP) && (timera[timePara+0][1] == -1))	/* 加速完成标志 */
		{
			if(StepRotateFork.TIMCount_Acc <= ((int)(StepRotateFork.accelTime*1000)))
			{
				tt = FunLine(startSpeed_count,StepRotateFork.countRate_Acc,StepRotateFork.TIMCount_Acc/1000);
				TIM_NUM->ARR = tt;
				if(timePara  == Rotate_Num)
				{
					TIM_NUM->CCR2 = tt/2;
				}
				else
				{
					TIM_NUM->CCR1 = tt/2;
				}
				StepRotateFork.TIMCount_Acc ++;
				timera[timePara+0][1] = 0;						/* 加速完成标志 */
				timera[timePara+0][3] = 0;						/* 加速计数标志 */
			}
			else 
			{
				StepRotateFork.TIMCount_Acc = 0;
				timera[timePara+0][0] = 0;									/* 加速使能标志 */
				StepRotateFork.motorState = CONSTANT;			
				timera[timePara+1][0] = -1;									/* 匀速使能标志 */
			}
		}
		
		if((StepRotateFork.motorState == CONSTANT) && (timera[timePara+1][1] == -1))		/* 匀速完成标志 */
		{
			timera[timePara+1][0] = 0;												/* 匀速使能标志 */
			timera[timePara+2][0] = -1;												/* 减速使能标志 */		
			StepRotateFork.motorState = DOWN;
		}
		
		if((StepRotateFork.motorState == DOWN) && (timera[timePara+2][1] == -1))/* 减速完成标志 */	
		{
			if(StepRotateFork.TIMCount_Dec <= ((int)(StepRotateFork.decelTime*1000)))
			{
				tt = FunLine_dec(StepRotateFork.max_speed_count ,StepRotateFork.countRate_Dec,StepRotateFork.TIMCount_Dec/1000);
				TIM_NUM->ARR = tt;
				if(timePara  == Rotate_Num)
				{
					TIM_NUM->CCR2 = tt/2;
				}
				else
				{
					TIM_NUM->CCR1 = tt/2;
				}
				StepRotateFork.TIMCount_Dec ++;
				timera[timePara+2][1] = 0;					/* 减速完成标志 */
				timera[timePara+2][3] = 0;					/* 减速计数标志 */
			}
			else 
			{
				StepRotateFork.motorState = WAIT;
				StepRotateFork.TIMCount_Dec = 0;
				timera[timePara+2][0] = 0;					/* 减速使能标志 */	
				memset(&StepRotateFork,0,sizeof(StepRotateFork));
			}
		}	
	}
}

/*  
*StepParameter:步进电机参数定义
step：步数
accel：加速度
decel：减速度
speed：速度
timePara：定时器参数 ：BigFork_Num、SmallFork_Num、Rotate_Num
DIR：方向
*/

/* 步进电机启动函数 */
void StepMotor_PWM(u8 timePara)
{
	switch(timePara)					/* 对计算数据进行赋值 */
	{
		case BigFork_Num:
		{ 
			if(StepBigFork.motorState == WAIT)
			{
				StepBigFork=stepMotor_Calculate(steptBigFork_Info.pulse,steptBigFork_Info.accV ,steptBigFork_Info.decV ,steptBigFork_Info.speed);						/* 计算运动参数 */;
				StepBigFork.motorState = UP;												/* 步进电机状态配置 */
				if(StepBigFork.moveMode == acc_constant_dec)	
				{
					BigFork_DIR = steptBigFork_Info.dir;
					BigForkSpeedCst(2) = (StepBigFork.constantTime ) * 1000;
					PWM_Output_B(startSpeed_count, StepBigFork.allStep);
					BigForkSpeedUp(0) = -1;	
				}
				
				if(StepBigFork.moveMode == acc_dec)
				{
					BigFork_DIR = steptBigFork_Info.dir;
					PWM_Output_B(startSpeed_count, StepBigFork.allStep);																						
					BigForkSpeedUp(0) = -1;	
				}
				memset(&steptBigFork_Info,0,sizeof(steptBigFork_Info));							/* 清除配置信息 */
			}
			else
			{
				/* 报错，步进电机上一个步骤还未完成，再次发送控制命令 */
			}
			break;
		}
		
		case SmallFork_Num:
		{
			if(StepSmallFork.motorState == WAIT)
			{
				StepSmallFork = stepMotor_Calculate(steptSmallFork_Info.pulse,steptSmallFork_Info.accV ,steptSmallFork_Info.decV ,steptSmallFork_Info.speed);						/* 计算运动参数 */;
				StepSmallFork.motorState = UP;																							/* 步进电机状态配置 */
				if(StepSmallFork.moveMode == acc_constant_dec)	
				{
					SmallFork_DIR = steptSmallFork_Info.dir;
					SmallForkSpeedCst(2) = (StepSmallFork.constantTime ) * 1000;
					PWM_Output_C(startSpeed_count, StepSmallFork.allStep);
					SmallForkSpeedUp(0) = -1;
				}
				if(StepSmallFork.moveMode == acc_dec)
				{
					SmallFork_DIR = steptSmallFork_Info.dir;
					PWM_Output_C(startSpeed_count, StepSmallFork.allStep);
					SmallForkSpeedUp(0) = -1;
				}
				memset(&steptSmallFork_Info,0,sizeof(steptSmallFork_Info));															/* 清除配置信息 */
			}
			else
			{
				/* 报错，步进电机上一个步骤还未完成，再次发送控制命令 */
			}
			break;
		}
		
		case Rotate_Num:
		{		
			if(StepRotateFork.motorState == WAIT)
			{
				StepRotateFork = stepMotor_Calculate(steptRotate_Info.pulse,steptRotate_Info.accV ,steptRotate_Info.decV ,steptRotate_Info.speed);						/* 计算运动参数 */;
				StepRotateFork.motorState = UP;
				if(StepRotateFork.moveMode == acc_constant_dec)	
				{
					Rotate_DIR = steptRotate_Info.dir;
					RotateSpeedCst(2) = (StepRotateFork.constantTime ) * 1000;
					PWM_Output_A(startSpeed_count, StepRotateFork.allStep);
					RotateSpeedUp(0) = -1;
				}
				if(StepRotateFork.moveMode == acc_dec)
				{
					Rotate_DIR = steptRotate_Info.dir;
					PWM_Output_A(startSpeed_count, StepRotateFork.allStep);
					RotateSpeedUp(0) = -1;
				}
				memset(&steptRotate_Info,0,sizeof(steptRotate_Info));	
			}
			else
			{
				/* 报错，步进电机上一个步骤还未完成，再次发送控制命令 */
			}
			break;
		}		
	}
}

/* 
	大叉、小叉顺时针向左
旋转电机方向与小叉方向相反
*/




/* 大叉伸缩动作函数 */
void BigFork_Extend_Left(void)																/* 大叉向左伸展 */
{
	if(RobotFinishFlag.BigFork_Extend_left_finish == 0xff)
	{
		RobotFinishFlag.BigFork_Extend_left_finish = 0;
	}
	RobotState.BigFork_Extend_left = 0xff;
	
	if(BigForkLimit_L == 0)																	/* 左极限触发禁止向左 */
	{
		RobotState.BigFork_Extend_left = 0;
	}
	else
	{
			BigFork_SteptMotor_Start(FlashConfig.BigFork_ES_Position,FlashConfig.BigFork_ES_V,FlashConfig.BigFork_ES_AV,FlashConfig.BigFork_ES_DV,negative);
	}
}

void BigFork_Shrink_Left(void)																/* 大叉由左收缩 */
{
	if(RobotFinishFlag.BigFork_Shrink_left_finish == 0xff)
	{
		RobotFinishFlag.BigFork_Shrink_left_finish = 0;
	}
	RobotState.BigFork_Shrink_left = 0xff;
	
	if(BigForkHome == 0)																	/* 原点触发禁止由左收缩 */
	{
		RobotState.BigFork_Shrink_left = 0;
	}
	else
	{
		BigFork_SteptMotor_Start(FlashConfig.BigFork_ES_Position,FlashConfig.BigFork_ES_V,FlashConfig.BigFork_ES_AV,FlashConfig.BigFork_ES_DV,Positive);
	}
}


void BigFork_Extend_Right(void)																/* 大叉向右伸展 */
{
	if(RobotFinishFlag.BigFork_Extend_right_finish == 0xff)
	{
		RobotFinishFlag.BigFork_Extend_right_finish = 0;
	}	
	RobotState.BigFork_Extend_right = 0xff;
	
	if(BigForkLimit_R == 0)																	/* 右极限触发禁止向右 */
	{
		RobotState.BigFork_Extend_right = 0;
	}
	else
	{
		BigFork_SteptMotor_Start(FlashConfig.BigFork_ES_Position,FlashConfig.BigFork_ES_V,FlashConfig.BigFork_ES_AV,FlashConfig.BigFork_ES_DV,Positive);
	}
}

void BigFork_Shrink_Right(void)																/* 大叉由右收缩 */
{
	if(RobotFinishFlag.BigFork_Shrink_right_finish == 0xff)
	{
		RobotFinishFlag.BigFork_Shrink_right_finish = 0;
	}		
	RobotState.BigFork_Shrink_right = 0xff;
	
	if(BigForkHome == 0)																	/* 原点触发禁止由右向左收缩 */
	{
		RobotState.BigFork_Shrink_right = 0;
	}
	else
	{
		BigFork_SteptMotor_Start(FlashConfig.BigFork_ES_Position,FlashConfig.BigFork_ES_V,FlashConfig.BigFork_ES_AV,FlashConfig.BigFork_ES_DV,negative);
	}
}

void SteptBigFork_ReturnHome(void)																					/* 步进大叉回原函数 */
{
	ClearBigForkExtendError();
		if(RobotFinishFlag.ReturnHome_State_finish[0] == 0xff)
		{
			RobotFinishFlag.ReturnHome_State_finish[0] = 0;
		}			
		RobotState.ReturnHome_State[0] = 0xff;
		RobotState.steptBigForkReturnHomeDir = 0;													/* 将回原方向清0，防止原点触发情况下回原BUG */
		
		if(BigForkHome == 0)
		{
			if(BigForkHome == 0)
			{
//				BigForkReturnHome1(0) = -1;				//等待错开原点再标记位置
				if(RobotState.ReturnHome_State[0] == 0xff)
				{
					RobotFinishFlag.ReturnHome_State_finish[0] = 0xff;							/* 找到原点，关闭电机，设置完成标志，清除状态标志 */
					CloseSteptMotor(BigFork_Num);
					RobotState.ReturnHome_State[0] = 0;
				}
			}
		}
		else
		{
			RobotState.BigForkBlindFindHome = 0xff;																		/* 进入盲找原点状态 */
			RobotState.steptBigForkReturnHomeDir = ReturnHome_Right;		/* 标记回原方向向右 */
			BigFork_SteptMotor_Start(BigFork_ReturnHome.ReturnHome_Position,BigFork_ReturnHome.ReturnHome_Speed,BigFork_ReturnHome.ReturnHome_Accel,BigFork_ReturnHome.ReturnHome_Decel,Positive);
		}
}


void	StepBigFork_Left_ReturnHomeFun(void)																				/* 大叉向左找原点 单独指令 */
{
	BigFork_SteptMotor_Start(BigFork_ReturnHome.ReturnHome_Position,BigFork_ReturnHome.ReturnHome_Speed,BigFork_ReturnHome.ReturnHome_Accel,BigFork_ReturnHome.ReturnHome_Decel,negative);
}

void	StepBigFork_Right_ReturnHomeFun(void)																				/* 大叉向右找原点 单独指令 */
{
	BigFork_SteptMotor_Start(BigFork_ReturnHome.ReturnHome_Position,BigFork_ReturnHome.ReturnHome_Speed,BigFork_ReturnHome.ReturnHome_Accel,BigFork_ReturnHome.ReturnHome_Decel,Positive);
}



/* 步进小叉伸缩动作函数 */

void SmallFork_Extend_Left(void)																/* 小叉伸展 */
{
	if(RobotFinishFlag.SmallFork_Extend_finish == 0xff)
	{
		RobotFinishFlag.SmallFork_Extend_finish = 0;
	}
	RobotState.SmallFork_Extend = 0xff;
	
	if(SmallForkLimit_L == 0)																	/* 左极限触发禁止向左 */
	{
		if(SmallForkLimit_L == 0)																	/* 二次判断左极限是否触发 */
		{
			RobotState.SmallFork_Extend = 0;												/* 左极限触发，禁止伸出 */
			
			/* 左极限触发，无法再向左伸出，报错 */
		}
	}
	else
	{
			SmallFork_SteptMotor_Start(FlashConfig.SmallFork_ES_Position,FlashConfig.SmallFork_ES_V,FlashConfig.SmallFork_ES_AV,FlashConfig.SmallFork_ES_DV,negative);
	}
	
//	SmallFork_SteptMotor_Start(FlashConfig.SmallFork_ES_Position,FlashConfig.SmallFork_ES_V,FlashConfig.SmallFork_ES_AV,FlashConfig.SmallFork_ES_DV,negative);
}

void SmallFork_Shrink_Left(void)																/* 小叉收缩 */
{
	if(RobotFinishFlag.SmallFork_Shrink_finish == 0xff)
	{
		RobotFinishFlag.SmallFork_Shrink_finish = 0;
	}
	RobotState.SmallFork_Shrink = 0xff;														/* 小叉收缩状态标记 */
	
	if(SmallForkHome == 0)																				/* 小叉原点触发禁止向收缩 */
	{
		if(SmallForkHome == 0)																			/* 二次确认小叉原点触发 */
		{
			RobotState.SmallFork_Shrink = 0;			
			/* 原点触发情况下，收到收缩指令，报错 */
		}
	}
	else
	{
			SmallFork_SteptMotor_Start(FlashConfig.SmallFork_ES_Position,FlashConfig.SmallFork_ES_V,FlashConfig.SmallFork_ES_AV,FlashConfig.SmallFork_ES_DV,Positive);
	}
}

void SteptSmallFork_ReturnHome(void)														/* 小叉回原 */
{
	ClearSmallForkExtendError();
	if(RobotFinishFlag.ReturnHome_State_finish[1] == 0xff)
	{
		RobotFinishFlag.ReturnHome_State_finish[1] = 0;
	}
	RobotState.ReturnHome_State[1] = 0xff;

	if(SmallForkHome == 0)																								/* 小叉回原信号触发 */
	{
		if(RobotState.ReturnHome_State[1] == 0xff)												/* 回原状态被标记 */
		{
			RobotFinishFlag.ReturnHome_State_finish[1] = 0xff;							/* 找到原点，关闭电机，设置完成标志，清除状态标志 */
			CloseSteptMotor(SmallFork_Num);
			RobotState.ReturnHome_State[1] = 0;
		}
	}
	else
	{
		SmallFork_SteptMotor_Start(SmallFork_Return.ReturnHome_Position,SmallFork_Return.ReturnHome_Speed,SmallFork_Return.ReturnHome_Accel,SmallFork_Return.ReturnHome_Decel,Positive);
	}

}


/* 旋转动作函数 */
void Rotate_Left_Out(void)																/* 向左转出*/
{
	if(RobotFinishFlag.Rotate_out_left_finish == 0xff)
	{
		RobotFinishFlag.Rotate_out_left_finish = 0;
	}	
	RobotState.Rotate_out_left = 0xff;
	
	if(abs(angle - FlashConfig.Rotate_Angle_Left) <= (Angle_Window+3))														/* 已经在左出的位置 */
	{
		RobotState.Rotate_out_left = 0;
		RobotFinishFlag.Rotate_out_left_finish  = 0xff;
	}
	else
	{
		Rotate_SteptMotor_Start(FlashConfig.Rotate_Position,FlashConfig.Rotate_V,FlashConfig.Rotate_AV,FlashConfig.Rotate_DV,negative);
	}
}

void Rotate_Left_Back(void)																/* 由左转回*/
{
	if(RobotFinishFlag.Rotate_back_left_finish == 0xff)
	{
		RobotFinishFlag.Rotate_back_left_finish = 0;
	}	
	RobotState.Rotate_back_left = 0xff;	
	if(abs(angle - FlashConfig.Rotate_Angle_Home) <= (Angle_Window+3))																	/* 原点触发 */
	{
		RobotState.Rotate_back_left = 0;
		RobotFinishFlag.Rotate_back_left_finish = 0xff;	
	}
	else
	{
		Rotate_SteptMotor_Start(FlashConfig.Rotate_Position,FlashConfig.Rotate_V,FlashConfig.Rotate_AV,FlashConfig.Rotate_DV,Positive);
	}
}

void Rotate_Right_Out(void)																/* 向右转出 */
{
	if(RobotFinishFlag.Rotate_out_right_finish == 0xff)
	{
		RobotFinishFlag.Rotate_out_right_finish = 0;
	}	
	RobotState.Rotate_out_right = 0xff;
	if(abs(angle - FlashConfig.Rotate_Angle_Right) <= (Angle_Window+3))																			/* 右极限触发 */
	{
		RobotState.Rotate_out_right = 0;
		RobotFinishFlag.Rotate_out_right_finish  = 0xff;
	}
	else
	{
		Rotate_SteptMotor_Start(FlashConfig.Rotate_Position,FlashConfig.Rotate_V,FlashConfig.Rotate_AV,FlashConfig.Rotate_DV,Positive);
	}
}

void Rotate_Right_Back(void)																/* 向右转回 */
{
	if(RobotFinishFlag.Rotate_back_right_finish == 0xff)
	{
		RobotFinishFlag.Rotate_back_right_finish = 0;
	}		
	RobotState.Rotate_back_right = 0xff;	
	if(abs(angle - FlashConfig.Rotate_Angle_Home) <= (Angle_Window+3))																	/* 原点触发禁止返回 */
	{
		RobotState.Rotate_back_right = 0;
		RobotFinishFlag.Rotate_back_right_finish = 0xff;
	}
	else
	{
		Rotate_SteptMotor_Start(FlashConfig.Rotate_Position + SteptRotateOvershootPosiition,FlashConfig.Rotate_V,FlashConfig.Rotate_AV,FlashConfig.Rotate_DV,negative);
	}
}


////////////////////////* 以下这个版本暂时未用， 没有修改*/
void Rotate_Behind_Out_left(void)
{
	if(RobotFinishFlag.Rotate_out_behind_left_finish == 0xff)
	{
		RobotFinishFlag.Rotate_out_behind_left_finish = 0;
	}	
	RobotState.Rotate_behind_out_left = 0xff;	
	Rotate_SteptMotor_Start(FlashConfig.Rotate_Position,FlashConfig.Rotate_V,FlashConfig.Rotate_AV,FlashConfig.Rotate_DV,negative);
}

void Rotate_Behind_Back_left(void)
{
	if(RobotFinishFlag.Rotate_back_behind_left_finish == 0xff)
	{
		RobotFinishFlag.Rotate_back_behind_left_finish = 0;
	}	
	RobotState.Rotate_behind_back_left = 0xff;	
	Rotate_SteptMotor_Start(FlashConfig.Rotate_Position  + SteptRotateOvershootPosiition,FlashConfig.Rotate_V,FlashConfig.Rotate_AV,FlashConfig.Rotate_DV,Positive);
}

void Rotate_Behind_Out_right(void)
{
	if(RobotFinishFlag.Rotate_out_behind_right_finish == 0xff)
	{
		RobotFinishFlag.Rotate_out_behind_right_finish = 0;
	}	
	RobotState.Rotate_behind_out_right = 0xff;	
	Rotate_SteptMotor_Start(FlashConfig.Rotate_Position,FlashConfig.Rotate_V,FlashConfig.Rotate_AV,FlashConfig.Rotate_DV,Positive);
}

void Rotate_Behind_Back_right(void)
{
	if(RobotFinishFlag.Rotate_back_behind_right_finish == 0xff)
	{
		RobotFinishFlag.Rotate_back_behind_right_finish = 0;
	}	
	RobotState.Rotate_behind_back_right = 0xff;	
	Rotate_SteptMotor_Start(FlashConfig.Rotate_Position  + SteptRotateOvershootPosiition,FlashConfig.Rotate_V,FlashConfig.Rotate_AV,FlashConfig.Rotate_DV, negative);
}
/////////////////////////////////////////以上///////////////////////////////////////////////////////////////

void Rotate_ReturnHome(void)																/* 旋转回原 */
{
	ClearRotateError();
	if(RobotFinishFlag.ReturnHome_State_finish[2] == 0xff)
	{
		RobotFinishFlag.ReturnHome_State_finish[2] = 0;
	}			
	RobotState.ReturnHome_State[2] = 0xff;
	if((angle - FlashConfig.Rotate_Angle_Home) < -Angle_Window)
	{
		Rotate_SteptMotor_Start(Rotate_Return.ReturnHome_Position,Rotate_Return.ReturnHome_Speed,Rotate_Return.ReturnHome_Accel,Rotate_Return.ReturnHome_Decel,Positive);
	}
	else if ((angle - FlashConfig.Rotate_Angle_Home) > Angle_Window)
	{
		Rotate_SteptMotor_Start(Rotate_Return.ReturnHome_Position,Rotate_Return.ReturnHome_Speed,Rotate_Return.ReturnHome_Accel,Rotate_Return.ReturnHome_Decel,negative);
	}
	else
	{
		RobotFinishFlag.ReturnHome_State_finish[2] = 0xff;
		RobotState.ReturnHome_State[2] = 0;
	}
}


/* 大叉伸缩启动函数 */
void	BigFork_SteptMotor_Start(float Position,float speed,float acc, float dec, u8 dir)
{
	if(RobotState.ReturnHome_State[0] == 0xff)
	{
		steptBigFork_Info.pulse = Position * spr * 8.67;
		steptBigFork_Info.accV = acc;
		steptBigFork_Info.decV = dec;
		steptBigFork_Info.speed = speed;
		steptBigFork_Info.dir = dir;
		steptBigFork_Info.startUp = 0xff;
	}
	else 
	{
		if(RobotState.SteptBigFork_LastDir == dir)																		/* 如果两次运行的方向相同，则不需增加，皮带张紧距离 */
		{
			steptBigFork_Info.pulse = Position * spr * 8.67;
			steptBigFork_Info.accV = acc;
			steptBigFork_Info.decV = dec;
			steptBigFork_Info.speed = speed;
			steptBigFork_Info.dir = dir;
			steptBigFork_Info.startUp = 0xff;
		}
		else																																					/* 否则增加皮带张紧距离 */
		{
			steptBigFork_Info.pulse = (Position + SteptBigforkPositionCompensation) * spr * 8.67;
			steptBigFork_Info.accV = acc;
			steptBigFork_Info.decV = dec;
			steptBigFork_Info.speed = speed;
			steptBigFork_Info.dir = dir;
			steptBigFork_Info.startUp = 0xff;
			RobotState.SteptBigFork_LastDir = dir;
		}
	}
}

/* 小叉伸缩启动函数 */
void	SmallFork_SteptMotor_Start(float Position,float speed,float acc, float dec, u8 dir)
{
	steptSmallFork_Info.pulse = Position * spr * 10;
	steptSmallFork_Info.accV = acc;
	steptSmallFork_Info.decV = dec;
	steptSmallFork_Info.speed = speed;
	steptSmallFork_Info.dir = dir;
	steptSmallFork_Info.startUp = 0xff;
}


/* 旋转启动函数 */
void	Rotate_SteptMotor_Start(float Position,float speed,float acc, float dec, u8 dir)
{
	if(RobotState.ReturnHome_State[2] == 0xff)
	{
		steptRotate_Info.pulse = Position * spr;
		steptRotate_Info.accV = acc;
		steptRotate_Info.decV = dec;
		steptRotate_Info.speed = speed;
		steptRotate_Info.dir = dir;
		steptRotate_Info.startUp = 0xff;
	}
	else
	{
		if(RobotState.SteptRotateFork_LastDir == dir)
		{
			steptRotate_Info.pulse = Position * spr;
			steptRotate_Info.accV = acc;
			steptRotate_Info.decV = dec;
			steptRotate_Info.speed = speed;
			steptRotate_Info.dir = dir;
			steptRotate_Info.startUp = 0xff;
		}
		else
		{
			steptRotate_Info.pulse = (Position + SteptRotatePositionCompensation) * spr;
			steptRotate_Info.accV = acc;
			steptRotate_Info.decV = dec;
			steptRotate_Info.speed = speed;
			steptRotate_Info.dir = dir;
			steptRotate_Info.startUp = 0xff;
			RobotState.SteptRotateFork_LastDir = dir;		
		}
	}
}

u32 CountSpeedChange(float speed)
{
	u32 count;
	count = count_speedPara / speed;
	return count;
}

float SpeedCountChange(u32 count)
{
	float speed;
	speed = count_speedPara / ((float)count);
	return speed;
}

float FunLine(float y0,float k,float x)
{
	float y = 0;
	y = y0 - k*x;
	return (y);
}

float FunLine_dec(float y0,float k,float x)
{
	float y = 0;
	y = y0 + k*x;
	return (y);
}



















