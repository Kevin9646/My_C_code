#include "sensor.h"
#include "delay.h" 


//struct SteptMotor_MoveState	SteptMotor_WorkState;

u8 BigForkNearSmallForkFlag	= 0;

u8 SaveBigForkSpeed = 0;
u8 SaveSmallForkSpeed = 0;

//��������ʼ������
void Sensor_Init(void)
{
	
	GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_GPIOE, ENABLE);//ʹ��ʱ��
 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15; //
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ����ģʽ
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//����
  GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��
	
	 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_3|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;//WK_UP��Ӧ����
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ����ģʽ
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//����
  GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ����ģʽ
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//����
  GPIO_Init(GPIOD, &GPIO_InitStructure);//��ʼ��	
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ����ģʽ
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//����
  GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4| GPIO_Pin_5 |GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ����ģʽ
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//����
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��	
} 


u8 Sensor_Scan(void)
{	 
	/* ---------------------------------------------------------------------С������			start */
	
	if(SmallForkHome == 0)																								/* С���ԭ�źŴ��� */
	{	
		if(SmallForkHome == 0)																							/* ����ȷ��С���ԭ�źŴ��� */
		{	
			if(RobotState.ReturnHome_State[1] == 0xff)												/* ��ԭ״̬����� */
			{
				RobotFinishFlag.ReturnHome_State_finish[1] = 0xff;							/* �ҵ�ԭ�㣬�رյ����������ɱ�־�����״̬��־ */
				CloseSteptMotor(SmallFork_Num);
				RobotState.ReturnHome_State[1] = 0;
			}
			
			// С�����ؼ�⵽������
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
			// С�������⵽������
			if(RobotState.SmallFork_Extend  == 0xff) 
			{
				CloseSteptMotor(SmallFork_Num);
				RobotState.SmallFork_Extend  = 0;
				RobotFinishFlag.SmallFork_Extend_finish  = 0xff;			
			}
		}
	}	
	
	/* -------------------------------------------------------------------С������			end */	
	
	/* ������� */
	if(BigForkHome == 0)																											/* ����ԭ�źŴ��� */
	{				
		if(BigForkHome == 0)																										/* ����ԭ�źŴ���������֤ */
		{	
			if(RobotState.ReturnHome_State[0] == 0xff)														/* ���ڻ�ԭ״̬ */
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
					RobotFinishFlag.ReturnHome_State_finish[0] = 0xff;									/* ������ɱ�־�����״̬��־ */
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
		if(BigForkLimit_L == 0)																										/* ����ȷ�����޴��� */
		{		
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
		if(RobotState.BigForkBlindFindHome == 0xff)
		{
			RobotState.BigForkBlindFindHome = 0;																	/* �ҵ����޺�ͱ�ɴ���������ԭ���ˣ������ä���ź� */
			CloseSteptMotor(BigFork_Num);
			BigFork_SteptMotor_Start(BigFork_ReturnHome.ReturnHome_Position,BigFork_ReturnHome.ReturnHome_Speed,BigFork_ReturnHome.ReturnHome_Accel,BigFork_ReturnHome.ReturnHome_Decel,negative);	/* ���������˶�����Ѱ��ԭ�� */			
			RobotState.steptBigForkReturnHomeDir = ReturnHome_Left;								/* ��Ǵ���ԭ���������Ҽ��ޣ�������ԭ�� */
			RobotState.steptBigForkReturnChangeDir = 0xff;			
		}	
		if(RobotState.BigFork_Extend_right  == 0xff) 
		{
			CloseSteptMotor(BigFork_Num);
			RobotState.BigFork_Extend_right  = 0;
			RobotFinishFlag.BigFork_Extend_right_finish  = 0xff;			
		}	
	}
		
	
	/* ��ת */
	if(abs(angle - FlashConfig.Rotate_Angle_Home) <= Angle_Window)
	{			
		if(RobotState.ReturnHome_State[2] == 0xff)
		{
			RobotFinishFlag.ReturnHome_State_finish[2] = 0xff;
			CloseSteptMotor(Rotate_Num);
			RobotState.ReturnHome_State[2] = 0;					
		}
		if(RobotState.Rotate_back_left  == 0xff) 
		{	
			CloseSteptMotor(Rotate_Num);				
			RobotState.Rotate_back_left = 0;
			RobotFinishFlag.Rotate_back_left_finish = 0xff;				
		}	
		if(RobotState.Rotate_back_right  == 0xff) 
		{
			CloseSteptMotor(Rotate_Num);		
			RobotState.Rotate_back_right = 0;
			RobotFinishFlag.Rotate_back_right_finish = 0xff;					
							
		}	
	}

	if(abs(angle - FlashConfig.Rotate_Angle_Right) <= Angle_Window)
	{
		if(RobotState.Rotate_out_right  == 0xff) 
		{
			CloseSteptMotor(Rotate_Num);
			RobotState.Rotate_out_right  = 0;
			RobotFinishFlag.Rotate_out_right_finish  = 0xff;	
		}
	}
	
	if(abs(angle - FlashConfig.Rotate_Angle_Left) <= Angle_Window)
	{
		if(RobotState.Rotate_out_left  == 0xff) 
		{
			CloseSteptMotor(Rotate_Num);
			RobotState.Rotate_out_left  = 0;
			RobotFinishFlag.Rotate_out_left_finish  = 0xff;
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

u8 SmallForkAllowExtend(void)
{
		// С���������ϰ���
		if(!SmallForkExtendDetect)
		{
			if(!SmallForkExtendDetect)
			{
				if(RobotState.SmallFork_Extend  == 0xff) 
				{
					CloseSteptMotor(SmallFork_Num);
					RobotState.SmallFork_Extend  = 0;
					SteptControlError.SmallFork_Detect_object_Error = 0xff;	
					RobotFinishFlag.SmallFork_Extend_finish  = 0xff;
				}
			}
		}
		return 0;
}

u8 BigForkAllowExtend(void)
{
	if(!BigForkExtendDetect_Left)
	{
		if(!BigForkExtendDetect_Left)
		{
			if(RobotState.BigFork_Extend_left  == 0xff) 													
			{
				CloseSteptMotor(BigFork_Num);
				RobotState.BigFork_Extend_left  = 0;
				RobotFinishFlag.BigFork_Extend_left_finish  = 0xff;	
				SteptControlError.BigFork_Detect_object_Left_Error = 0xff;
			}
		}
	}
	
	if(!BigForkExtendDetect_Right)
	{
		if(!BigForkExtendDetect_Right)
		{
			if(RobotState.BigFork_Extend_right  == 0xff) 
			{
				CloseSteptMotor(BigFork_Num);
				RobotState.BigFork_Extend_right  = 0;
				RobotFinishFlag.BigFork_Extend_right_finish  = 0xff;
				SteptControlError.BigFork_Detect_object_Right_Error = 0xff;			
			}
		}
	}
	return 0;
}



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










