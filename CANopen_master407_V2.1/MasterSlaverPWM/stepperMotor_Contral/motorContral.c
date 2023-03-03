#include "motorContral.h"
#include "math.h"

struct StepMotorInitDef  StepSmallFork;					/* �������С��������� */
struct StepMotorInitDef  StepBigFork;						/* ����������������� */
struct StepMotorInitDef  StepRotateFork;				/* ���������ת�������� */
struct StepReturnHome	BigFork_ReturnHome,SmallFork_Return,Rotate_Return;					/* ��ԭ���� */

/* 	
	stepMotor_Calculate���˶���������
	step������ 
	accel�����ٶ� 			0.5��ʾ0.5Ȧ/s2����һ��0.5Ȧ/s,�ڶ���1Ȧ/s
	decel�����ٶ� 
	speed������ٶ�
*/

struct StepMotorInitDef  stepMotor_Calculate(u32 step,float accel,float decel, float speed)
{
	
	float i;
	float stepnCount = 0;										/* ��n���ļ���ֵ */
	struct StepMotorInitDef Parameter;
	memset(&Parameter,0,sizeof(Parameter));
	
	Parameter.allStep = step;
	Parameter.max_speed_count = CountSpeedChange(speed);					/* ���ٵ�����ٶȵļ���ֵ */
	Parameter.accelTime = (speed -  startSpeed) / accel;		/* �������ʱ�� */
	Parameter.countRate_Acc = (startSpeed_count - Parameter.max_speed_count) / Parameter.accelTime;   /* �������ı仯�ʼ��� */
	
	/* ������ٲ��� */
	for(i=0;i<(int)(Parameter.accelTime*1000);i++)
	{
		stepnCount = FunLine(startSpeed_count,Parameter.countRate_Acc,i/1000);
		Parameter.accel_step = Parameter.accel_step + (u16)(accel_T/((stepnCount/ft)*1000)); 
	}
	
	Parameter.decelTime = (speed - startSpeed)  / decel;		/* �������ʱ�� */
	Parameter.countRate_Dec = (startSpeed_count - Parameter.max_speed_count) / Parameter.decelTime;   /* �������ı仯�ʼ��� */
	
		/* ������ٲ��� */
	for(i=0;i<(int)(Parameter.decelTime*1000);i++)
	{
		stepnCount = FunLine_dec(Parameter.max_speed_count ,Parameter.countRate_Dec,i/1000);
		Parameter.decel_step = Parameter.decel_step + (u16)(accel_T/((stepnCount/ft)*1000)); 
	}
	
	if((Parameter.accel_step + Parameter.decel_step) > Parameter.allStep)		/* �ж��Ƿ��ܹ����ٵ�����ٶ� */
	{
		Parameter.acc_dec_t = (speed - startSpeed) / (accel + decel);
//		/* ��������������ٶȼ���ֵ */
		Parameter.max_speed_count = FunLine(startSpeed_count,Parameter.countRate_Acc,Parameter.acc_dec_t);
		Parameter.decelTime = (SpeedCountChange(Parameter.max_speed_count) - startSpeed) / decel;
		Parameter.accelTime = Parameter.acc_dec_t;		
		
		Parameter.moveMode = acc_dec;
	}
	else
	{
		Parameter.constant_step = Parameter.allStep - (Parameter.accel_step + Parameter.decel_step);									/* �������ٲ��� */
		Parameter.constantTime = Parameter.constant_step * (((float)(Parameter.max_speed_count))/ft) ;			/* ����ʱ�� */
		Parameter.moveMode = acc_constant_dec;
	}

	/* �������ʱ������ֵ */
	return Parameter;
}

/*

��涨ʱ��

timePara: 0 3 6  �ֱ�Ϊ��桢С�桢��ת
TIM_NUM:	TIM_BigFork/TIM_SmallFork/TIM_Rotate
*/
/* motorContral(&StepSmallFork,SmallFork_No,TIM_SmallFork); */
void SteptMotor_SoferTime_Big(u8 timePara, TIM_TypeDef *TIM_NUM)
{
	u16 tt =0;
	
	if(StepBigFork.moveMode == acc_dec)					/* �޷��ﵽ���ʱ�ͼ��� */
	{
		if((StepBigFork.motorState == UP) && (BigForkSpeedUp(1) == -1))	/* ������ɱ�־ */
		{
			if(StepBigFork.TIMCount_Acc <= ((int)(StepBigFork.accelTime*1000)))
			{
				tt = FunLine(startSpeed_count,StepBigFork.countRate_Acc,StepBigFork.TIMCount_Acc/1000);
				TIM_NUM->ARR = tt;
				
				TIM_NUM->CCR1 = tt/2;																	/* �߼���ʱ��ΪCCR2 */
	
				StepBigFork.TIMCount_Acc = StepBigFork.TIMCount_Acc + 1;
				BigForkSpeedUp(1) = 0;						/* ������ɱ�־ */
				BigForkSpeedUp(3) = 0;						/* ���ټ�����־ */
			}
			else 
			{
				StepBigFork.TIMCount_Acc = 0;
				BigForkSpeedUp(0) = 0;									/* ����ʹ�ܱ�־ */
				StepBigFork.motorState = DOWN;			
				BigForkSpeedDown(0) = -1;									/* ����ʹ�ܱ�־ */
			}
		}
		
		if((StepBigFork.motorState == DOWN) && (BigForkSpeedDown(1) == -1))/* ������ɱ�־ */	
		{
			if(StepBigFork.TIMCount_Dec <= ((int)(StepBigFork.decelTime*1000)))
			{
				tt = FunLine_dec(StepBigFork.max_speed_count,StepBigFork.countRate_Dec,StepBigFork.TIMCount_Dec/1000);
				TIM_NUM->ARR = tt;

					TIM_NUM->CCR1 = tt/2;


				StepBigFork.TIMCount_Dec = StepBigFork.TIMCount_Dec + 1;
				BigForkSpeedDown(1) = 0;					/* ������ɱ�־ */
				BigForkSpeedDown(3) = 0;					/* ���ټ�����־ */
			}
			else 
			{
				
				StepBigFork.motorState = WAIT;
				StepBigFork.TIMCount_Dec = 0;
				BigForkSpeedDown(0) = 0;					/* ����ʹ�ܱ�־ */
				memset(&StepBigFork,0,sizeof(StepBigFork));
			}
		}	
	}

  if(StepBigFork.moveMode == acc_constant_dec)																		/* �������ٺͼ���  */
	{
		if((StepBigFork.motorState == UP) && (BigForkSpeedUp(1) == -1))	/* ������ɱ�־ */
		{
			if(StepBigFork.TIMCount_Acc <= ((int)(StepBigFork.accelTime*1000)))
			{
				tt = FunLine(startSpeed_count,StepBigFork.countRate_Acc,StepBigFork.TIMCount_Acc/1000);
				TIM_NUM->ARR = tt;

					TIM_NUM->CCR1 = tt/2;

				StepBigFork.TIMCount_Acc = StepBigFork.TIMCount_Acc + 1;
				BigForkSpeedUp(1) = 0;						/* ������ɱ�־ */
				BigForkSpeedUp(3) = 0;						/* ���ټ�����־ */
			}
			else 
			{
				StepBigFork.TIMCount_Acc = 0;
				BigForkSpeedUp(0) = 0;									/* ����ʹ�ܱ�־ */
				StepBigFork.motorState = CONSTANT;			
				BigForkSpeedCst(0) = -1;									/* ����ʹ�ܱ�־ */
			}
		}
		
		if((StepBigFork.motorState == CONSTANT) && (BigForkSpeedCst(1) == -1))		/* ������ɱ�־ */
		{
			BigForkSpeedCst(0) = 0;												/* ����ʹ�ܱ�־ */
			BigForkSpeedDown(0) = -1;												/* ����ʹ�ܱ�־ */		
			StepBigFork.motorState = DOWN;
		}
		
		if((StepBigFork.motorState == DOWN) && (BigForkSpeedDown(1) == -1))/* ������ɱ�־ */	
		{
			if(StepBigFork.TIMCount_Dec <= ((int)(StepBigFork.decelTime*1000)))
			{
				tt = FunLine_dec(StepBigFork.max_speed_count ,StepBigFork.countRate_Dec,StepBigFork.TIMCount_Dec/1000);
				TIM_NUM->ARR = tt;

					TIM_NUM->CCR1 = tt/2;
				

				StepBigFork.TIMCount_Dec = StepBigFork.TIMCount_Dec + 1;
				BigForkSpeedDown(1) = 0;					/* ������ɱ�־ */
				BigForkSpeedDown(3) = 0;					/* ���ټ�����־ */
			}
			else 
			{
				StepBigFork.motorState = WAIT;
				StepBigFork.TIMCount_Dec = 0;
				BigForkSpeedDown(0) = 0;					/* ����ʹ�ܱ�־ */
	
				memset(&StepBigFork,0,sizeof(StepBigFork));
			}
		}	
	}
}


/* С����ת��ʱ�� */

void SteptMotor_SoferTime_Small(u8 timePara, TIM_TypeDef *TIM_NUM)
{
	u16 tt =0;
	
	if(StepSmallFork.moveMode == acc_dec)					/* �޷��ﵽ���ʱ�ͼ��� */
	{
		if((StepSmallFork.motorState == UP) && (timera[timePara+0][1] == -1))	/* ������ɱ�־ */
		{
			if(StepSmallFork.TIMCount_Acc <= ((int)(StepSmallFork.accelTime*1000)))
			{
				tt = FunLine(startSpeed_count,StepSmallFork.countRate_Acc,StepSmallFork.TIMCount_Acc/1000);
				TIM_NUM->ARR = tt;
				if(timePara  == Rotate_Num)
				{
					TIM_NUM->CCR2 = tt/2;																	/* �߼���ʱ��ΪCCR2 */
				}
				else
				{
					TIM_NUM->CCR1 = tt/2;																	/* ͨ�ö�ʱ��ΪCCR1 */
				}
				
				StepSmallFork.TIMCount_Acc ++;
				timera[timePara+0][1] = 0;						/* ������ɱ�־ */
				timera[timePara+0][3] = 0;						/* ���ټ�����־ */
			}
			else 
			{
				StepSmallFork.TIMCount_Acc = 0;
				timera[timePara+0][0] = 0;									/* ����ʹ�ܱ�־ */
				StepSmallFork.motorState = DOWN;			
				timera[timePara+2][0] = -1;									/* ����ʹ�ܱ�־ */
			}
		}
		
		if((StepSmallFork.motorState == DOWN) && (timera[timePara+2][1] == -1))/* ������ɱ�־ */	
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
				timera[timePara+2][1] = 0;					/* ������ɱ�־ */
				timera[timePara+2][3] = 0;					/* ���ټ�����־ */
			}
			else 
			{
				StepSmallFork.motorState = WAIT;
				StepSmallFork.TIMCount_Dec = 0;
				timera[timePara+2][0] = 0;					/* ����ʹ�ܱ�־ */
				memset(&StepSmallFork,0,sizeof(StepSmallFork));
			}
		}	
	}
	
	
	
  if(StepSmallFork.moveMode == acc_constant_dec)																		/* �������ٺͼ���  */
	{
		if((StepSmallFork.motorState == UP) && (timera[timePara+0][1] == -1))	/* ������ɱ�־ */
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
				timera[timePara+0][1] = 0;						/* ������ɱ�־ */
				timera[timePara+0][3] = 0;						/* ���ټ�����־ */
			}
			else 
			{
				StepSmallFork.TIMCount_Acc = 0;
				timera[timePara+0][0] = 0;									/* ����ʹ�ܱ�־ */
				StepSmallFork.motorState = CONSTANT;			
				timera[timePara+1][0] = -1;									/* ����ʹ�ܱ�־ */
			}
		}
		
		if((StepSmallFork.motorState == CONSTANT) && (timera[timePara+1][1] == -1))		/* ������ɱ�־ */
		{
			timera[timePara+1][0] = 0;												/* ����ʹ�ܱ�־ */
			timera[timePara+2][0] = -1;												/* ����ʹ�ܱ�־ */		
			StepSmallFork.motorState = DOWN;
		}
		
		if((StepSmallFork.motorState == DOWN) && (timera[timePara+2][1] == -1))/* ������ɱ�־ */	
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
				timera[timePara+2][1] = 0;					/* ������ɱ�־ */
				timera[timePara+2][3] = 0;					/* ���ټ�����־ */
			}
			else 
			{
				StepSmallFork.motorState = WAIT;	
				StepSmallFork.TIMCount_Dec = 0;
				timera[timePara+2][0] = 0;					/* ����ʹ�ܱ�־ */			
				memset(&StepSmallFork,0,sizeof(StepSmallFork));
				
			}
		}	
	}
}

/* ��ת�����ʱ�� */

void SteptMotor_SoferTime_Rotate(u8 timePara, TIM_TypeDef *TIM_NUM)
{
	u16 tt =0;
	
	if(StepRotateFork.moveMode == acc_dec)					/* �޷��ﵽ���ʱ�ͼ��� */
	{
		if((StepRotateFork.motorState == UP) && (timera[timePara+0][1] == -1))	/* ������ɱ�־ */
		{
			if(StepRotateFork.TIMCount_Acc <= ((int)(StepRotateFork.accelTime*1000)))
			{
				tt = FunLine(startSpeed_count,StepRotateFork.countRate_Acc,StepRotateFork.TIMCount_Acc/1000);
				TIM_NUM->ARR = tt;
				if(timePara  == Rotate_Num)
				{
					TIM_NUM->CCR2 = tt/2;																	/* �߼���ʱ��ΪCCR2 */
				}
				else
				{
					TIM_NUM->CCR1 = tt/2;																	/* ͨ�ö�ʱ��ΪCCR1 */
				}
				
				StepRotateFork.TIMCount_Acc ++;
				timera[timePara+0][1] = 0;						/* ������ɱ�־ */
				timera[timePara+0][3] = 0;						/* ���ټ�����־ */
			}
			else 
			{
				StepRotateFork.TIMCount_Acc = 0;
				timera[timePara+0][0] = 0;									/* ����ʹ�ܱ�־ */
				StepRotateFork.motorState = DOWN;			
				timera[timePara+2][0] = -1;									/* ����ʹ�ܱ�־ */
			}
		}
		
		if((StepRotateFork.motorState == DOWN) && (timera[timePara+2][1] == -1))/* ������ɱ�־ */	
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
				timera[timePara+2][1] = 0;					/* ������ɱ�־ */
				timera[timePara+2][3] = 0;					/* ���ټ�����־ */
			}
			else 
			{
				StepRotateFork.motorState = WAIT;
				StepRotateFork.TIMCount_Dec = 0;
				timera[timePara+2][0] = 0;					/* ����ʹ�ܱ�־ */	
				memset(&StepRotateFork,0,sizeof(StepRotateFork));
			}
		}	
	}
	
	
  if(StepRotateFork.moveMode == acc_constant_dec)																		/* �������ٺͼ���  */
	{
		if((StepRotateFork.motorState == UP) && (timera[timePara+0][1] == -1))	/* ������ɱ�־ */
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
				timera[timePara+0][1] = 0;						/* ������ɱ�־ */
				timera[timePara+0][3] = 0;						/* ���ټ�����־ */
			}
			else 
			{
				StepRotateFork.TIMCount_Acc = 0;
				timera[timePara+0][0] = 0;									/* ����ʹ�ܱ�־ */
				StepRotateFork.motorState = CONSTANT;			
				timera[timePara+1][0] = -1;									/* ����ʹ�ܱ�־ */
			}
		}
		
		if((StepRotateFork.motorState == CONSTANT) && (timera[timePara+1][1] == -1))		/* ������ɱ�־ */
		{
			timera[timePara+1][0] = 0;												/* ����ʹ�ܱ�־ */
			timera[timePara+2][0] = -1;												/* ����ʹ�ܱ�־ */		
			StepRotateFork.motorState = DOWN;
		}
		
		if((StepRotateFork.motorState == DOWN) && (timera[timePara+2][1] == -1))/* ������ɱ�־ */	
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
				timera[timePara+2][1] = 0;					/* ������ɱ�־ */
				timera[timePara+2][3] = 0;					/* ���ټ�����־ */
			}
			else 
			{
				StepRotateFork.motorState = WAIT;
				StepRotateFork.TIMCount_Dec = 0;
				timera[timePara+2][0] = 0;					/* ����ʹ�ܱ�־ */	
				memset(&StepRotateFork,0,sizeof(StepRotateFork));
			}
		}	
	}
}

/*  
*StepParameter:���������������
step������
accel�����ٶ�
decel�����ٶ�
speed���ٶ�
timePara����ʱ������ ��BigFork_Num��SmallFork_Num��Rotate_Num
DIR������
*/

/* ��������������� */
void StepMotor_PWM(u8 timePara)
{
	switch(timePara)					/* �Լ������ݽ��и�ֵ */
	{
		case BigFork_Num:
		{ 
			if(StepBigFork.motorState == WAIT)
			{
				StepBigFork=stepMotor_Calculate(steptBigFork_Info.pulse,steptBigFork_Info.accV ,steptBigFork_Info.decV ,steptBigFork_Info.speed);						/* �����˶����� */;
				StepBigFork.motorState = UP;												/* �������״̬���� */
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
				memset(&steptBigFork_Info,0,sizeof(steptBigFork_Info));							/* ���������Ϣ */
			}
			else
			{
				/* �������������һ�����軹δ��ɣ��ٴη��Ϳ������� */
			}
			break;
		}
		
		case SmallFork_Num:
		{
			if(StepSmallFork.motorState == WAIT)
			{
				StepSmallFork = stepMotor_Calculate(steptSmallFork_Info.pulse,steptSmallFork_Info.accV ,steptSmallFork_Info.decV ,steptSmallFork_Info.speed);						/* �����˶����� */;
				StepSmallFork.motorState = UP;																							/* �������״̬���� */
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
				memset(&steptSmallFork_Info,0,sizeof(steptSmallFork_Info));															/* ���������Ϣ */
			}
			else
			{
				/* �������������һ�����軹δ��ɣ��ٴη��Ϳ������� */
			}
			break;
		}
		
		case Rotate_Num:
		{		
			if(StepRotateFork.motorState == WAIT)
			{
				StepRotateFork = stepMotor_Calculate(steptRotate_Info.pulse,steptRotate_Info.accV ,steptRotate_Info.decV ,steptRotate_Info.speed);						/* �����˶����� */;
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
				/* �������������һ�����軹δ��ɣ��ٴη��Ϳ������� */
			}
			break;
		}		
	}
}

/* 
	��桢С��˳ʱ������
��ת���������С�淽���෴
*/




/* ��������������� */
void BigFork_Extend_Left(void)																/* ���������չ */
{
	if(RobotFinishFlag.BigFork_Extend_left_finish == 0xff)
	{
		RobotFinishFlag.BigFork_Extend_left_finish = 0;
	}
	RobotState.BigFork_Extend_left = 0xff;
	
	if(BigForkLimit_L == 0)																	/* ���޴�����ֹ���� */
	{
		RobotState.BigFork_Extend_left = 0;
	}
	else
	{
			BigFork_SteptMotor_Start(FlashConfig.BigFork_ES_Position,FlashConfig.BigFork_ES_V,FlashConfig.BigFork_ES_AV,FlashConfig.BigFork_ES_DV,negative);
	}
}

void BigFork_Shrink_Left(void)																/* ����������� */
{
	if(RobotFinishFlag.BigFork_Shrink_left_finish == 0xff)
	{
		RobotFinishFlag.BigFork_Shrink_left_finish = 0;
	}
	RobotState.BigFork_Shrink_left = 0xff;
	
	if(BigForkHome == 0)																	/* ԭ�㴥����ֹ�������� */
	{
		RobotState.BigFork_Shrink_left = 0;
	}
	else
	{
		BigFork_SteptMotor_Start(FlashConfig.BigFork_ES_Position,FlashConfig.BigFork_ES_V,FlashConfig.BigFork_ES_AV,FlashConfig.BigFork_ES_DV,Positive);
	}
}


void BigFork_Extend_Right(void)																/* ���������չ */
{
	if(RobotFinishFlag.BigFork_Extend_right_finish == 0xff)
	{
		RobotFinishFlag.BigFork_Extend_right_finish = 0;
	}	
	RobotState.BigFork_Extend_right = 0xff;
	
	if(BigForkLimit_R == 0)																	/* �Ҽ��޴�����ֹ���� */
	{
		RobotState.BigFork_Extend_right = 0;
	}
	else
	{
		BigFork_SteptMotor_Start(FlashConfig.BigFork_ES_Position,FlashConfig.BigFork_ES_V,FlashConfig.BigFork_ES_AV,FlashConfig.BigFork_ES_DV,Positive);
	}
}

void BigFork_Shrink_Right(void)																/* ����������� */
{
	if(RobotFinishFlag.BigFork_Shrink_right_finish == 0xff)
	{
		RobotFinishFlag.BigFork_Shrink_right_finish = 0;
	}		
	RobotState.BigFork_Shrink_right = 0xff;
	
	if(BigForkHome == 0)																	/* ԭ�㴥����ֹ������������ */
	{
		RobotState.BigFork_Shrink_right = 0;
	}
	else
	{
		BigFork_SteptMotor_Start(FlashConfig.BigFork_ES_Position,FlashConfig.BigFork_ES_V,FlashConfig.BigFork_ES_AV,FlashConfig.BigFork_ES_DV,negative);
	}
}

void SteptBigFork_ReturnHome(void)																					/* ��������ԭ���� */
{
	ClearBigForkExtendError();
		if(RobotFinishFlag.ReturnHome_State_finish[0] == 0xff)
		{
			RobotFinishFlag.ReturnHome_State_finish[0] = 0;
		}			
		RobotState.ReturnHome_State[0] = 0xff;
		RobotState.steptBigForkReturnHomeDir = 0;													/* ����ԭ������0����ֹԭ�㴥������»�ԭBUG */
		
		if(BigForkHome == 0)
		{
			if(BigForkHome == 0)
			{
//				BigForkReturnHome1(0) = -1;				//�ȴ���ԭ���ٱ��λ��
				if(RobotState.ReturnHome_State[0] == 0xff)
				{
					RobotFinishFlag.ReturnHome_State_finish[0] = 0xff;							/* �ҵ�ԭ�㣬�رյ����������ɱ�־�����״̬��־ */
					CloseSteptMotor(BigFork_Num);
					RobotState.ReturnHome_State[0] = 0;
				}
			}
		}
		else
		{
			RobotState.BigForkBlindFindHome = 0xff;																		/* ����ä��ԭ��״̬ */
			RobotState.steptBigForkReturnHomeDir = ReturnHome_Right;		/* ��ǻ�ԭ�������� */
			BigFork_SteptMotor_Start(BigFork_ReturnHome.ReturnHome_Position,BigFork_ReturnHome.ReturnHome_Speed,BigFork_ReturnHome.ReturnHome_Accel,BigFork_ReturnHome.ReturnHome_Decel,Positive);
		}
}


void	StepBigFork_Left_ReturnHomeFun(void)																				/* ���������ԭ�� ����ָ�� */
{
	BigFork_SteptMotor_Start(BigFork_ReturnHome.ReturnHome_Position,BigFork_ReturnHome.ReturnHome_Speed,BigFork_ReturnHome.ReturnHome_Accel,BigFork_ReturnHome.ReturnHome_Decel,negative);
}

void	StepBigFork_Right_ReturnHomeFun(void)																				/* ���������ԭ�� ����ָ�� */
{
	BigFork_SteptMotor_Start(BigFork_ReturnHome.ReturnHome_Position,BigFork_ReturnHome.ReturnHome_Speed,BigFork_ReturnHome.ReturnHome_Accel,BigFork_ReturnHome.ReturnHome_Decel,Positive);
}



/* ����С�������������� */

void SmallFork_Extend_Left(void)																/* С����չ */
{
	if(RobotFinishFlag.SmallFork_Extend_finish == 0xff)
	{
		RobotFinishFlag.SmallFork_Extend_finish = 0;
	}
	RobotState.SmallFork_Extend = 0xff;
	
	if(SmallForkLimit_L == 0)																	/* ���޴�����ֹ���� */
	{
		if(SmallForkLimit_L == 0)																	/* �����ж������Ƿ񴥷� */
		{
			RobotState.SmallFork_Extend = 0;												/* ���޴�������ֹ��� */
			
			/* ���޴������޷���������������� */
		}
	}
	else
	{
			SmallFork_SteptMotor_Start(FlashConfig.SmallFork_ES_Position,FlashConfig.SmallFork_ES_V,FlashConfig.SmallFork_ES_AV,FlashConfig.SmallFork_ES_DV,negative);
	}
	
//	SmallFork_SteptMotor_Start(FlashConfig.SmallFork_ES_Position,FlashConfig.SmallFork_ES_V,FlashConfig.SmallFork_ES_AV,FlashConfig.SmallFork_ES_DV,negative);
}

void SmallFork_Shrink_Left(void)																/* С������ */
{
	if(RobotFinishFlag.SmallFork_Shrink_finish == 0xff)
	{
		RobotFinishFlag.SmallFork_Shrink_finish = 0;
	}
	RobotState.SmallFork_Shrink = 0xff;														/* С������״̬��� */
	
	if(SmallForkHome == 0)																				/* С��ԭ�㴥����ֹ������ */
	{
		if(SmallForkHome == 0)																			/* ����ȷ��С��ԭ�㴥�� */
		{
			RobotState.SmallFork_Shrink = 0;			
			/* ԭ�㴥������£��յ�����ָ����� */
		}
	}
	else
	{
			SmallFork_SteptMotor_Start(FlashConfig.SmallFork_ES_Position,FlashConfig.SmallFork_ES_V,FlashConfig.SmallFork_ES_AV,FlashConfig.SmallFork_ES_DV,Positive);
	}
}

void SteptSmallFork_ReturnHome(void)														/* С���ԭ */
{
	ClearSmallForkExtendError();
	if(RobotFinishFlag.ReturnHome_State_finish[1] == 0xff)
	{
		RobotFinishFlag.ReturnHome_State_finish[1] = 0;
	}
	RobotState.ReturnHome_State[1] = 0xff;

	if(SmallForkHome == 0)																								/* С���ԭ�źŴ��� */
	{
		if(RobotState.ReturnHome_State[1] == 0xff)												/* ��ԭ״̬����� */
		{
			RobotFinishFlag.ReturnHome_State_finish[1] = 0xff;							/* �ҵ�ԭ�㣬�رյ����������ɱ�־�����״̬��־ */
			CloseSteptMotor(SmallFork_Num);
			RobotState.ReturnHome_State[1] = 0;
		}
	}
	else
	{
		SmallFork_SteptMotor_Start(SmallFork_Return.ReturnHome_Position,SmallFork_Return.ReturnHome_Speed,SmallFork_Return.ReturnHome_Accel,SmallFork_Return.ReturnHome_Decel,Positive);
	}

}


/* ��ת�������� */
void Rotate_Left_Out(void)																/* ����ת��*/
{
	if(RobotFinishFlag.Rotate_out_left_finish == 0xff)
	{
		RobotFinishFlag.Rotate_out_left_finish = 0;
	}	
	RobotState.Rotate_out_left = 0xff;
	
	if(abs(angle - FlashConfig.Rotate_Angle_Left) <= (Angle_Window+3))														/* �Ѿ��������λ�� */
	{
		RobotState.Rotate_out_left = 0;
		RobotFinishFlag.Rotate_out_left_finish  = 0xff;
	}
	else
	{
		Rotate_SteptMotor_Start(FlashConfig.Rotate_Position,FlashConfig.Rotate_V,FlashConfig.Rotate_AV,FlashConfig.Rotate_DV,negative);
	}
}

void Rotate_Left_Back(void)																/* ����ת��*/
{
	if(RobotFinishFlag.Rotate_back_left_finish == 0xff)
	{
		RobotFinishFlag.Rotate_back_left_finish = 0;
	}	
	RobotState.Rotate_back_left = 0xff;	
	if(abs(angle - FlashConfig.Rotate_Angle_Home) <= (Angle_Window+3))																	/* ԭ�㴥�� */
	{
		RobotState.Rotate_back_left = 0;
		RobotFinishFlag.Rotate_back_left_finish = 0xff;	
	}
	else
	{
		Rotate_SteptMotor_Start(FlashConfig.Rotate_Position,FlashConfig.Rotate_V,FlashConfig.Rotate_AV,FlashConfig.Rotate_DV,Positive);
	}
}

void Rotate_Right_Out(void)																/* ����ת�� */
{
	if(RobotFinishFlag.Rotate_out_right_finish == 0xff)
	{
		RobotFinishFlag.Rotate_out_right_finish = 0;
	}	
	RobotState.Rotate_out_right = 0xff;
	if(abs(angle - FlashConfig.Rotate_Angle_Right) <= (Angle_Window+3))																			/* �Ҽ��޴��� */
	{
		RobotState.Rotate_out_right = 0;
		RobotFinishFlag.Rotate_out_right_finish  = 0xff;
	}
	else
	{
		Rotate_SteptMotor_Start(FlashConfig.Rotate_Position,FlashConfig.Rotate_V,FlashConfig.Rotate_AV,FlashConfig.Rotate_DV,Positive);
	}
}

void Rotate_Right_Back(void)																/* ����ת�� */
{
	if(RobotFinishFlag.Rotate_back_right_finish == 0xff)
	{
		RobotFinishFlag.Rotate_back_right_finish = 0;
	}		
	RobotState.Rotate_back_right = 0xff;	
	if(abs(angle - FlashConfig.Rotate_Angle_Home) <= (Angle_Window+3))																	/* ԭ�㴥����ֹ���� */
	{
		RobotState.Rotate_back_right = 0;
		RobotFinishFlag.Rotate_back_right_finish = 0xff;
	}
	else
	{
		Rotate_SteptMotor_Start(FlashConfig.Rotate_Position + SteptRotateOvershootPosiition,FlashConfig.Rotate_V,FlashConfig.Rotate_AV,FlashConfig.Rotate_DV,negative);
	}
}


////////////////////////* ��������汾��ʱδ�ã� û���޸�*/
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
/////////////////////////////////////////����///////////////////////////////////////////////////////////////

void Rotate_ReturnHome(void)																/* ��ת��ԭ */
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


/* ��������������� */
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
		if(RobotState.SteptBigFork_LastDir == dir)																		/* ����������еķ�����ͬ���������ӣ�Ƥ���Ž����� */
		{
			steptBigFork_Info.pulse = Position * spr * 8.67;
			steptBigFork_Info.accV = acc;
			steptBigFork_Info.decV = dec;
			steptBigFork_Info.speed = speed;
			steptBigFork_Info.dir = dir;
			steptBigFork_Info.startUp = 0xff;
		}
		else																																					/* ��������Ƥ���Ž����� */
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

/* С�������������� */
void	SmallFork_SteptMotor_Start(float Position,float speed,float acc, float dec, u8 dir)
{
	steptSmallFork_Info.pulse = Position * spr * 10;
	steptSmallFork_Info.accV = acc;
	steptSmallFork_Info.decV = dec;
	steptSmallFork_Info.speed = speed;
	steptSmallFork_Info.dir = dir;
	steptSmallFork_Info.startUp = 0xff;
}


/* ��ת�������� */
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



















