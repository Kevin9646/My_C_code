#include "softtimer.h"


//ͨ�ö�ʱ��7�жϳ�ʼ��
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=��ʱ������Ƶ��,��λ:Mhz
//����ʹ�õ��Ƕ�ʱ��7,��Ƶ84Mhz��

/* �����ʱ��ȫ�ּ������� */



u32 timerpcnt = 0;

/* 
	timera[i][0]:������ʱ����־
	timera[i][1]����ʱ��ɱ�־
	timera[i][2]����ʱĿ��ֵ
	timera[i][3]����ʱ����ֵ
*/
int32_t timera[TIMER_][4]=
{
	{0,0,accel_T,0,},					// ������0
	{0,0,0,0,},								// ������1
	{0,0,accel_T,0,},					// �������2
	{0,0,accel_T,0,},					// С�����3	       
	{0,0,0,0,},								// С�����4
	{0,0,accel_T,0,},					// С������5
	{0,0,accel_T,0,},					// ��ת����6
	{0,0,0,0,},								// ��ת����7
	{0,0,accel_T,0,},					// ��ת����8	
	{0,0,500,0},							/* 485����9 */
	{0,0,200,0},							/* �������ֻ���ʱ��10 */
	{0,0,200,0},							/* С������ֻ���ʱ��11 */
	{0,0,500,0},							/* ���ԭ�㻺��ʱ��12 */
	{0,0,500,0},							/* С��ԭ�㻺��ʱ��13 */
	{0,0,500,0},							/* ������ϻ���ʱ��14 */	
	{0,0,1000,0},							/* ������ϻ���ʱ��15 */	
	{0,0,500,0},							/* ����ԭ�Ż�ʱ��16 */
	{0,0,500,0},							/* С���ԭ�Ż�ʱ��17 */	
	{0,0,500,0},							/* ��ת��ԭ�Ż�ʱ��18 */	
	{0,0,1000,0},							/* ����ԭ�Ż�ʱ��19 */
	{0,0,1000,0},							/* С���ԭ�Ż�ʱ��20 */	
	{0,0,1000,0},								/* ��ת��ԭ�Ż�ʱ��21 */
	{0,0,5000,0},              /* �����ʱ���22*/
	{0,0,1000,0},                /* ��ת������λ23*/
	{0,0,10,0},                /* 10ms��ʱ24*/
	{0,0,5,0},                /* 5ms��ʱ25*/
	{0,0,1,0}                /* 1ms��ʱ26*/
};

void softTimer_Init(u16 arr,u16 psc)									/* TIM7   ���������ʱ�� */
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7,ENABLE);  ///ʹ��TIM7ʱ��
	
  TIM_TimeBaseInitStructure.TIM_Period = arr; 	//�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM7,&TIM_TimeBaseInitStructure);//��ʼ��TIM7
	
	TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE); //����ʱ��3�����ж�
	TIM_Cmd(TIM7,ENABLE); //ʹ�ܶ�ʱ��3
	
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM7_IRQn; //��ʱ��3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x00; //��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x01; //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}


//int aaa = 0;

//��ʱ��7�жϷ�����
void TIM7_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM7,TIM_IT_Update)==SET) //����ж�
	{
		TIM_ClearITPendingBit( TIM7, TIM_IT_Update);
		timerp();
		
	}
	TIM_ClearITPendingBit(TIM7,TIM_IT_Update);  //����жϱ�־λ
}

void timerp(void)
{
    int i;
    timerpcnt++;
    for (i=0;i<TIMER_;i++)
    {
        if (timera[i][0]<0)
        {
            if (timera[i][1]>=0)
            {
                timera[i][3]++;
                timera[i][1]=(timera[i][3]>=timera[i][2])?-1:0;
            };
        }
        else
        {
            timera[i][1]=0;													/* ��timer[i][0]>=0ʱ�������ʱ����0 */

            timera[i][3]=0;
        };
    };
}


bool ServoMotorTimeOut(void)
{
	if (ServoMotor(0) == 0)
	{
		ServoMotor(0) = -1;
	}
	if (ServoMotor(1) == -1)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void DisableServoMotorTimeOut(void)
{
	ServoMotor(0) = 0;
}



void	ScanSoftTimerState(void)																																																						/* ɨ�������ʱ������ */
{
	if(BigForkReturnHome(1) == -1)																	/* ����ԭ������һ�Σ��ٷ��� */
	{
		BigForkReturnHome(0) = 0;
		BigForkReturnHome(1) = 0;
		CloseSteptMotor(BigFork_Num);
		BigFork_SteptMotor_Start(BigFork_ReturnHome.ReturnHome_Position,BigFork_ReturnHome.ReturnHome_Speed,BigFork_ReturnHome.ReturnHome_Accel,BigFork_ReturnHome.ReturnHome_Decel,Positive);
		RobotState.steptBigForkReturnHomeDir = ReturnHome_Right;																																			/* ��ǻ�ԭ�������� */
	}
	
	if(BigForkReturnHome1(1) == -1)																					/* ԭ���źŴ�������»�ԭ�����߳�ԭ���źŷ�Χ���ٱ��״̬λ */
	{
		RobotState.BigForkBlindFindHome = 0xff;																		/* ����ä��ԭ��״̬ */
		RobotState.steptBigForkReturnHomeDir = ReturnHome_Right;		/* ��ǻ�ԭ�������� */
		BigForkReturnHome1(0) = 0;
		BigForkReturnHome1(1) = 0;
	}
	/* ��ת */
	if(RotateReturnHome(1) == -1)																	/* ����ԭ������һ�Σ��ٷ��� */
	{
		RotateReturnHome(0) = 0;
		RotateReturnHome(1) = 0;
		CloseSteptMotor(Rotate_Num);
		Rotate_SteptMotor_Start(Rotate_Return.ReturnHome_Position,Rotate_Return.ReturnHome_Speed,Rotate_Return.ReturnHome_Accel,Rotate_Return.ReturnHome_Decel,Positive);
		RobotState.steptRotateReturnHomeDir = ReturnHome_Right;																																			/* ��ǻ�ԭ�������� */
	}
	
	if(RotateReturnHome1(1) == -1)																					/* ԭ���źŴ�������»�ԭ�����߳�ԭ���źŷ�Χ���ٱ��״̬λ */
	{
		RobotState.RotateBlindFindHome = 0xff;																		/* ����ä��ԭ��״̬ */
		RobotState.steptRotateReturnHomeDir = ReturnHome_Right;										/* ��ǻ�ԭ�������� */
		RotateReturnHome1(0) = 0;
		RotateReturnHome1(1) = 0;
	}
	if(RotateMotorPoistion(1) == -1) 
	{
		CloseSteptMotor(Rotate_Num);
		RotateMotorPoistion(0) = 0;
		if (RobotState.Rotate_back_right  == 0xff)
		{
			SteptControlError.Rotate_RightBack_Error = 0xff;
			RobotState.Rotate_back_right = 0;
			RobotFinishFlag.Rotate_back_right_finish = 0xff;	
		}
		if(RobotState.Rotate_behind_back_left == 0xff)
		{
			SteptControlError.Rotate_LeftBehindBack_Error = 0xff;
			RobotState.Rotate_behind_back_left = 0;
			RobotFinishFlag.Rotate_back_behind_left_finish = 0xff;	
		}
		if(RobotState.Rotate_behind_back_right == 0xff)
		{
			SteptControlError.Rotate_RightBehindBack_Error = 0xff;
			RobotState.Rotate_behind_back_right = 0;
			RobotFinishFlag.Rotate_back_behind_right_finish = 0xff;		
		}
	}
}


