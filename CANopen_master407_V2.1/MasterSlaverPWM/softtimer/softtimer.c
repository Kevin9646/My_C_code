#include "softtimer.h"


//通用定时器7中断初始化
//arr：自动重装值。
//psc：时钟预分频数
//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=定时器工作频率,单位:Mhz
//这里使用的是定时器7,主频84Mhz。

/* 软件定时器全局计数变量 */



u32 timerpcnt = 0;

/* 
	timera[i][0]:启动定时器标志
	timera[i][1]：定时完成标志
	timera[i][2]：定时目标值
	timera[i][3]：定时计数值
*/
int32_t timera[TIMER_][4]=
{
	{0,0,accel_T,0,},					// 大叉加速0
	{0,0,0,0,},								// 大叉减速1
	{0,0,accel_T,0,},					// 大叉匀速2
	{0,0,accel_T,0,},					// 小叉加速3	       
	{0,0,0,0,},								// 小叉减速4
	{0,0,accel_T,0,},					// 小叉匀速5
	{0,0,accel_T,0,},					// 旋转加速6
	{0,0,0,0,},								// 旋转减速7
	{0,0,accel_T,0,},					// 旋转匀速8	
	{0,0,500,0},							/* 485保护9 */
	{0,0,200,0},							/* 大叉控制字缓冲时间10 */
	{0,0,200,0},							/* 小叉控制字缓冲时间11 */
	{0,0,500,0},							/* 大叉原点缓冲时间12 */
	{0,0,500,0},							/* 小叉原点缓冲时间13 */
	{0,0,500,0},							/* 清除故障缓冲时间14 */	
	{0,0,1000,0},							/* 清除故障缓冲时间15 */	
	{0,0,500,0},							/* 大叉回原优化时间16 */
	{0,0,500,0},							/* 小叉回原优化时间17 */	
	{0,0,500,0},							/* 旋转回原优化时间18 */	
	{0,0,1000,0},							/* 大叉回原优化时间19 */
	{0,0,1000,0},							/* 小叉回原优化时间20 */	
	{0,0,1000,0},								/* 旋转回原优化时间21 */
	{0,0,5000,0},              /* 舵机超时检测22*/
	{0,0,1000,0},                /* 旋转过量找位23*/
	{0,0,10,0},                /* 10ms定时24*/
	{0,0,5,0},                /* 5ms定时25*/
	{0,0,1,0}                /* 1ms定时26*/
};

void softTimer_Init(u16 arr,u16 psc)									/* TIM7   用作软件定时器 */
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7,ENABLE);  ///使能TIM7时钟
	
  TIM_TimeBaseInitStructure.TIM_Period = arr; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM7,&TIM_TimeBaseInitStructure);//初始化TIM7
	
	TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE); //允许定时器3更新中断
	TIM_Cmd(TIM7,ENABLE); //使能定时器3
	
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM7_IRQn; //定时器3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x00; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x01; //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}


//int aaa = 0;

//定时器7中断服务函数
void TIM7_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM7,TIM_IT_Update)==SET) //溢出中断
	{
		TIM_ClearITPendingBit( TIM7, TIM_IT_Update);
		timerp();
		
	}
	TIM_ClearITPendingBit(TIM7,TIM_IT_Update);  //清除中断标志位
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
            timera[i][1]=0;													/* 当timer[i][0]>=0时，软件定时器清0 */

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



void	ScanSoftTimerState(void)																																																						/* 扫描软件定时器汇总 */
{
	if(BigForkReturnHome(1) == -1)																	/* 碰到原点后多走一段，再反向 */
	{
		BigForkReturnHome(0) = 0;
		BigForkReturnHome(1) = 0;
		CloseSteptMotor(BigFork_Num);
		BigFork_SteptMotor_Start(BigFork_ReturnHome.ReturnHome_Position,BigFork_ReturnHome.ReturnHome_Speed,BigFork_ReturnHome.ReturnHome_Accel,BigFork_ReturnHome.ReturnHome_Decel,Positive);
		RobotState.steptBigForkReturnHomeDir = ReturnHome_Right;																																			/* 标记回原方向向右 */
	}
	
	if(BigForkReturnHome1(1) == -1)																					/* 原点信号触发情况下回原，等走出原点信号范围后再标记状态位 */
	{
		RobotState.BigForkBlindFindHome = 0xff;																		/* 进入盲找原点状态 */
		RobotState.steptBigForkReturnHomeDir = ReturnHome_Right;		/* 标记回原方向向右 */
		BigForkReturnHome1(0) = 0;
		BigForkReturnHome1(1) = 0;
	}
	/* 旋转 */
	if(RotateReturnHome(1) == -1)																	/* 碰到原点后多走一段，再反向 */
	{
		RotateReturnHome(0) = 0;
		RotateReturnHome(1) = 0;
		CloseSteptMotor(Rotate_Num);
		Rotate_SteptMotor_Start(Rotate_Return.ReturnHome_Position,Rotate_Return.ReturnHome_Speed,Rotate_Return.ReturnHome_Accel,Rotate_Return.ReturnHome_Decel,Positive);
		RobotState.steptRotateReturnHomeDir = ReturnHome_Right;																																			/* 标记回原方向向右 */
	}
	
	if(RotateReturnHome1(1) == -1)																					/* 原点信号触发情况下回原，等走出原点信号范围后再标记状态位 */
	{
		RobotState.RotateBlindFindHome = 0xff;																		/* 进入盲找原点状态 */
		RobotState.steptRotateReturnHomeDir = ReturnHome_Right;										/* 标记回原方向向右 */
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


