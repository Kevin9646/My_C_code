#include "PWMTimer.h"

/*********** Group A ***********/
// 定时器1主模式
/*
	u16 TIM1_Prescaler：TIM分频系数
	u16 TIM1_Period：   自动重装载周期
	u16 CCR_A						
*/
void TIM8_GPIO_Config(u16 TIM8_Prescaler, u16 TIM8_Period, u16 CCR_A)																			/* 旋转 */
{
	GPIO_InitTypeDef	GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_TIM8, ENABLE);
	RCC_AHB1PeriphClockCmd(	RCC_AHB1Periph_GPIOC, ENABLE);			// F407: GPIO 串口初始化 与F103不同
	
	GPIO_PinAFConfig( GPIOC, GPIO_PinSource7, GPIO_AF_TIM8);		// F407 端口复用映射

	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;		// TIM1_CH1 - PE13， CH4 - PE14
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;			// 复用
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		// 推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;			
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	TIM_TimeBaseInitTypeDef		TIM_TimeBaseStructure;
	TIM_TimeBaseStructure.TIM_Period = TIM8_Period - 1;
	TIM_TimeBaseStructure.TIM_Prescaler = TIM8_Prescaler - 1;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;			// 重复计数，一定要 = 0
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);
	
	TIM_OCInitTypeDef		TIM_OCInitStructure;
	// 设置工作模式
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;							// PWM1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	// 比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;			// 输出极性
	// PWM通道，TIM1 - 通道2设置函数，50/100
	TIM_OCInitStructure.TIM_Pulse = CCR_A;								// 设置待装入捕获寄存器的脉冲值
	TIM_OC2Init( TIM8, &TIM_OCInitStructure);
	TIM_SelectMasterSlaveMode( TIM8, TIM_MasterSlaveMode_Enable);
	TIM_SelectOutputTrigger( TIM8, TIM_TRGOSource_Update);
	TIM_OC2PreloadConfig( TIM8, TIM_OCPreload_Enable);
	
	TIM_ARRPreloadConfig(TIM8, ENABLE);
}

//定时器2从模式
void TIM2_GPIO_Config(u32 PulseNum_A)
{
	TIM_TimeBaseInitTypeDef	TIM_TimeBaseStructure;
	NVIC_InitTypeDef	NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM2, ENABLE);
	
	TIM_TimeBaseStructure.TIM_Period = PulseNum_A;
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit( TIM2, &TIM_TimeBaseStructure);
	
	TIM_SelectInputTrigger( TIM2, TIM_TS_ITR1);			// TIM1-主，TIM2-从
	TIM_SelectSlaveMode( TIM2, TIM_SlaveMode_External1);
	TIM_ITConfig( TIM2, TIM_IT_Update, DISABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 6;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init( &NVIC_InitStructure);
}
	
void PWM_Output_A(u16 Cycle_A, u32 PulseNum_A)			// TIM1-主，TIM2-从											/* 旋转 */
{
	TIM2_GPIO_Config(PulseNum_A);
	TIM_Cmd( TIM2, ENABLE);
	TIM_ClearITPendingBit( TIM2, TIM_IT_Update);
	TIM_ITConfig( TIM2, TIM_IT_Update, ENABLE);
	TIM8_GPIO_Config( 42, Cycle_A, Cycle_A / 2);	// F407: 高级定时器是 168MHz, 故168MHz / 84 = 2MHz
	TIM_Cmd( TIM8, ENABLE);
	TIM_CtrlPWMOutputs( TIM8, ENABLE);	// 高级定时器 TIM1 使能 MOE
}


/*********** Group B ***********/
// 定时器4主模式	
void TIM4_GPIO_Config(u16 TIM4_Prescaler, u16 TIM4_Period, u16 CCR_B)																		/* 大叉 */												
{
	GPIO_InitTypeDef	GPIO_InitStructure;
	
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM4, ENABLE);
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOD, ENABLE);			// F407: GPIO 串口初始化
	
	GPIO_PinAFConfig( GPIOD, GPIO_PinSource12, GPIO_AF_TIM4);		// F407 端口复用映射

	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;			// TIM4_CH1 PD12
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;			// 复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOD, &GPIO_InitStructure);	
	
	TIM_TimeBaseInitTypeDef		TIM_TimeBaseStructure;
	// 时钟频率设置
	TIM_TimeBaseStructure.TIM_Period = TIM4_Period - 1;
	TIM_TimeBaseStructure.TIM_Prescaler = TIM4_Prescaler - 1;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit( TIM4, &TIM_TimeBaseStructure);
	
	TIM_OCInitTypeDef		TIM_OCInitStructure;
	// 设置工作模式
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;			// 设置工作模式是PWM，且为PWM1工作模式，TIMx_CNT<TIMx_CCR1时为高电平
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;		// 也就是使能PWM输出到端口					
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;			// 输出极性
	// PWM通道，TIM4 - 通道1设置函数，50/100
	TIM_OCInitStructure.TIM_Pulse = CCR_B;					// 设置待装入捕获寄存器的脉冲值
	TIM_OC1Init( TIM4, &TIM_OCInitStructure);						// 初始化 TIM4-OC1
	TIM_SelectMasterSlaveMode( TIM4, TIM_MasterSlaveMode_Enable);		// 定时器主从模式使能
	TIM_SelectOutputTrigger( TIM4, TIM_TRGOSource_Update);						// 选择触发方式：使用更新事件作为触发输出
	TIM_OC1PreloadConfig( TIM4, TIM_OCPreload_Enable);		// CH1预装载使能，修改				

	
	TIM_ARRPreloadConfig( TIM4, ENABLE);					// 使能ARR预装载寄存器
}

//定时器1从模式
void TIM1_GPIO_Config(u32 PulseNum_B)
{
	TIM_TimeBaseInitTypeDef	TIM_TimeBaseStructure;
	NVIC_InitTypeDef	NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_TIM1, ENABLE);
	
	TIM_TimeBaseStructure.TIM_Period = PulseNum_B;
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit( TIM1, &TIM_TimeBaseStructure);
	
	TIM_SelectInputTrigger( TIM1, TIM_TS_ITR3);			// TIM2-主，TIM4-从
	TIM_SelectSlaveMode( TIM1, TIM_SlaveMode_External1);
	TIM_ITConfig( TIM1, TIM_IT_Update, DISABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_TIM10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 7;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init( &NVIC_InitStructure);
}

void PWM_Output_B(u16 Cycle_B, u32 PulseNum_B)			// TIM4-主，TIM1-从								/* 大叉 */
{
	TIM1_GPIO_Config(PulseNum_B);
	TIM_Cmd( TIM1, ENABLE);
	TIM_ClearITPendingBit( TIM1, TIM_IT_Update);
	TIM_ITConfig( TIM1, TIM_IT_Update, ENABLE);
	TIM4_GPIO_Config( 21, Cycle_B, Cycle_B / 2);		//F407:通用定时器是 84MHz
	TIM_Cmd( TIM4, ENABLE);
}



/*********** Group C         小叉   ***********/
// 定时器3主模式
void TIM3_GPIO_Config(u16 TIM3_Prescaler, u16 TIM3_Period, u16 CCR_C)																			/* 小叉 */
{
	GPIO_InitTypeDef	GPIO_InitStructure;
	// TIM3通道1\2 - PWM Z轴步进电机脉冲信号
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM3, ENABLE);
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOC, ENABLE);			// F407: GPIO 串口初始化
	
	GPIO_PinAFConfig( GPIOC, GPIO_PinSource6, GPIO_AF_TIM3);		// F407 端口复用映射

	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;			// TIM3_CH2 PC7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;			// 复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOC, &GPIO_InitStructure);		
	
	
	
	
	TIM_TimeBaseInitTypeDef		TIM_TimeBaseStructure;
	// 时钟频率设置
	TIM_TimeBaseStructure.TIM_Period = TIM3_Period - 1;
	TIM_TimeBaseStructure.TIM_Prescaler = TIM3_Prescaler - 1;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit( TIM3, &TIM_TimeBaseStructure);
	
	TIM_OCInitTypeDef		TIM_OCInitStructure;
	// 设置工作模式
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;			// 设置工作模式是PWM，且为PWM1工作模式，TIMx_CNT<TIMx_CCR1时为高电平
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;		// 也就是使能PWM输出到端口					
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;			// 输出极性
	// PWM通道，TIM3 - 通道1设置函数，50/100
	TIM_OCInitStructure.TIM_Pulse = CCR_C;					// 设置待装入捕获寄存器的脉冲值
	TIM_OC1Init( TIM3, &TIM_OCInitStructure);						// 初始化 TIM3-OC2
	TIM_SelectMasterSlaveMode( TIM3, TIM_MasterSlaveMode_Enable);		// 定时器主从模式使能
	TIM_SelectOutputTrigger( TIM3, TIM_TRGOSource_Update);						// 选择触发方式：使用更新事件作为触发输出
	TIM_OC1PreloadConfig( TIM3, TIM_OCPreload_Enable);		// CH2预装载使能，修改				

	TIM_ARRPreloadConfig( TIM3, ENABLE);					// 使能ARR预装载寄存器

}

// 定时器5从模式
void TIM5_GPIO_Config(u32 PulseNum_Z)
{
	TIM_TimeBaseInitTypeDef	TIM_TimeBaseStructure;
	NVIC_InitTypeDef	NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
	
	TIM_TimeBaseStructure.TIM_Period = PulseNum_Z;
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit( TIM5, &TIM_TimeBaseStructure);
	
	TIM_SelectInputTrigger( TIM5, TIM_TS_ITR1);
	TIM_SelectSlaveMode( TIM5,TIM_SlaveMode_External1 );		// 等同下一句 TIM5->SMCR |= 0x07
	TIM_ITConfig( TIM5, TIM_IT_Update, DISABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 8;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void PWM_Output_C(u16 Cycle_C, u32 PulseNum_C)			// TIM3-主，TIM5-从											/* 小叉PWM输出 */
{
	TIM5_GPIO_Config(PulseNum_C);
	TIM_Cmd( TIM5, ENABLE);
	TIM_ClearITPendingBit( TIM5, TIM_IT_Update);
	TIM_ITConfig( TIM5, TIM_IT_Update, ENABLE);
	TIM3_GPIO_Config( 21, Cycle_C, Cycle_C / 2);			//F407:通用定时器是 84MHz,
	TIM_Cmd( TIM3, ENABLE);
}


// Group A
void TIM2_IRQHandler(void)																										/* 旋转 */
{
	if (TIM_GetITStatus( TIM2, TIM_IT_Update) != RESET)
	{
		CloseSteptMotor(Rotate_Num);
		if(RobotState.ReturnHome_State[2] == 0xff) 
		{
			/*	回原步数走完，未找到原点，直接报错	*/
			SteptControlError.Rotate_ReturnHome_Error = 0xff;
		}
		
		if(RobotState.Rotate_out_left  == 0xff) 
		{
			RobotState.Rotate_out_left  = 0;
			
			if(abs(angle - FlashConfig.Rotate_Angle_Left) > Angle_Window)
			{
				SteptControlError.Rotate_LeftOut_Error = 0xff;
			}
			else
			{
				RobotFinishFlag.Rotate_out_left_finish  = 0xff;
			}
		}
		if(RobotState.Rotate_out_right  == 0xff) 
		{
			RobotState.Rotate_out_right  = 0;
			
			if(abs(angle - FlashConfig.Rotate_Angle_Right) > Angle_Window)
			{
				SteptControlError.Rotate_RightOut_Error = 0xff;	
			}
			else
			{
				RobotFinishFlag.Rotate_out_right_finish  = 0xff;
			}
		}	
		if(RobotState.Rotate_back_right  == 0xff)
		{
			RobotState.Rotate_back_right = 0;
			
			if(abs(angle - FlashConfig.Rotate_Angle_Home) > Angle_Window)
			{
				SteptControlError.Rotate_RightBack_Error = 0xff;
			}
			else
			{
				RobotFinishFlag.Rotate_back_right_finish = 0xff;
			}				
		}
		if(RobotState.Rotate_back_left  == 0xff) 
		{
			RobotState.Rotate_back_left = 0;
			if(abs(angle - FlashConfig.Rotate_Angle_Home) > Angle_Window)
			{
				SteptControlError.Rotate_LeftBack_Error = 0xff;
			}
			else
			{
				RobotFinishFlag.Rotate_back_left_finish = 0xff;
			}
		}
	}
}


// Group B
void TIM1_UP_TIM10_IRQHandler(void)																						/*大叉*/
{
	if (TIM_GetITStatus( TIM1, TIM_IT_Update) != RESET)
	{
		CloseSteptMotor(BigFork_Num);
		if(RobotState.ReturnHome_State[0] == 0xff) 
		{
				/* 回原状态步数跑完，直接报错 */
			SteptControlError.BigFork_ReturnHome_Error = 0xff;
		}

		if(RobotState.BigFork_Extend_left  == 0xff) 													
		{
			RobotState.BigFork_Extend_left  = 0;
			if(BigForkLimit_L != 0)	
			{
				SteptControlError.BigFork_LeftExtend_Error = 0xff;
				/* 报错 */
			}
			else
			{
				RobotFinishFlag.BigFork_Extend_left_finish  = 0xff;	
			}
		}
		if(RobotState.BigFork_Extend_right  == 0xff) 
		{
			RobotState.BigFork_Extend_right  = 0;
			if(BigForkLimit_R != 0)	
			{
				SteptControlError.BigFork_RightExtend_Error = 0xff;
				/* 报错 */
			}
			else
			{
				RobotFinishFlag.BigFork_Extend_right_finish  = 0xff;	
			}
		}	
		if(RobotState.BigFork_Shrink_right  == 0xff)
		{
			RobotState.BigFork_Shrink_right = 0;
			if(BigForkHome != 0)
			{
				SteptControlError.BigFork_RighShrink_Error = 0xff;
				/* 报错 */
			}
			else
			{
				RobotFinishFlag.BigFork_Shrink_right_finish = 0xff;	
			}
		}

		if(RobotState.BigFork_Shrink_left  == 0xff) 
		{
			RobotState.BigFork_Shrink_left = 0;
			if(BigForkHome != 0)
			{
				SteptControlError.BigFork_LeftShrink_Error = 0xff;
				/* 报错 */
			}
			else
			{
				RobotFinishFlag.BigFork_Shrink_left_finish = 0xff;
			}
		}
		if(RobotState.BigForkBlindFindHome == 0xff)																			/* 盲找原点步数用完，回原出错 */
		{
			SteptControlError.BigFork_ReturnHome_Error = 0xff;
		}	
	}
}

// Group C
void TIM5_IRQHandler(void)																										/* 小叉 */
{
	if (TIM_GetITStatus( TIM5, TIM_IT_Update) != RESET)
	{
		CloseSteptMotor(SmallFork_Num);
		if(RobotState.ReturnHome_State[1] == 0xff) 
		{
			/* 如果小叉回原步数走完，还没回原，直接报错 */
			SteptControlError.SmallFork_ReturnHome_Error = 0xff;
			
		}
		if(RobotState.SmallFork_Extend  == 0xff) 
		{
			RobotState.SmallFork_Extend  = 0;
			if(SmallForkLimit_L != 0)
			{
				SteptControlError.SmallFork_LeftExtend_Error = 0xff;
				/* 报错 */
			}
			else
			{
				RobotFinishFlag.SmallFork_Extend_finish  = 0xff;
			}		
		}
			
		if(RobotState.SmallFork_Shrink  == 0xff)
		{
			RobotState.SmallFork_Shrink = 0;
			if(SmallForkHome != 0)
			{
				SteptControlError.SmallFork_LeftShrink_Error = 0xff;
				/* 报错 */
			}
			else
			{
				RobotFinishFlag.SmallFork_Shrink_finish = 0xff;	
			}	
		}	
	}
}

void	TIM_GPIO_Init(void)
{
	
	/* 小叉 */
	
	GPIO_InitTypeDef	GPIO_InitStructure1;
	// TIM3通道1\2 - PWM Z轴步进电机脉冲信号
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM3, ENABLE);
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOC, ENABLE);			// F407: GPIO 串口初始化
	
	GPIO_PinAFConfig( GPIOC, GPIO_PinSource6, GPIO_AF_TIM3);		// F407 端口复用映射
	
	GPIO_InitStructure1.GPIO_Pin = GPIO_Pin_6;			// TIM3_CH2 PC7
	GPIO_InitStructure1.GPIO_Mode = GPIO_Mode_AF;			// 复用推挽输出
	GPIO_InitStructure1.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure1.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure1.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOC, &GPIO_InitStructure1);	
	
	
	
	/* 旋转 */
	GPIO_InitTypeDef	GPIO_InitStructure2;
	
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM4, ENABLE);
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOD, ENABLE);			// F407: GPIO 串口初始化
	
	GPIO_PinAFConfig( GPIOD, GPIO_PinSource12, GPIO_AF_TIM4);		// F407 端口复用映射

	
	GPIO_InitStructure2.GPIO_Pin = GPIO_Pin_12;			// TIM4_CH1 PD12
	GPIO_InitStructure2.GPIO_Mode = GPIO_Mode_AF;			// 复用推挽输出
	GPIO_InitStructure2.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure2.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure2.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOD, &GPIO_InitStructure2);	
	
	
	/* 大叉 */
	GPIO_InitTypeDef	GPIO_InitStructure3;
	
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_TIM8, ENABLE);
	RCC_AHB1PeriphClockCmd(	RCC_AHB1Periph_GPIOC, ENABLE);			// F407: GPIO 串口初始化 与F103不同
	
	GPIO_PinAFConfig( GPIOC, GPIO_PinSource7, GPIO_AF_TIM8);		// F407 端口复用映射

	
	GPIO_InitStructure3.GPIO_Pin = GPIO_Pin_7;		// TIM1_CH1 - PE13， CH4 - PE14
	GPIO_InitStructure3.GPIO_Mode = GPIO_Mode_AF;			// 复用
	GPIO_InitStructure3.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure3.GPIO_OType = GPIO_OType_PP;		// 推挽复用输出
	GPIO_InitStructure3.GPIO_PuPd = GPIO_PuPd_DOWN;			
	GPIO_Init(GPIOC, &GPIO_InitStructure3);
}




void CloseSteptMotor(u8 steptMotor_num)
{
	switch(steptMotor_num)
	{
		case BigFork_Num:
		{
			
			TIM_ClearITPendingBit( TIM1, TIM_IT_Update);			// 清除中断标志位
			TIM_Cmd( TIM4, DISABLE);			// 关闭定时器4
			TIM_Cmd( TIM1, DISABLE);			// 关闭定时器1
			TIM_ITConfig( TIM1, TIM_IT_Update, DISABLE);
			
			/* 以下为清除状态 */
			memset(&StepBigFork,0,sizeof(StepBigFork));	
			TIM_GPIO_Init();																				/* 初始化GPIO */
		/* 清除软件定时器状态 start */			
			BigForkSpeedUp(0) = 0;
			BigForkSpeedCst(0) = 0;
			BigForkSpeedDown(0) = 0;
			BigForkSpeedUp(1) = 0;
			BigForkSpeedCst(1) = 0;
			BigForkSpeedDown(1) = 0;				
			BigForkSpeedUp(3) = 0;
			BigForkSpeedCst(3) = 0;
			BigForkSpeedDown(3) = 0;				
		/* 清除软件定时器状态 end */			
			break;
		}
		case SmallFork_Num:
		{
			TIM_ClearITPendingBit( TIM5, TIM_IT_Update);			// 清除中断标志位
			TIM_Cmd( TIM3, DISABLE);			// 关闭定时器3
			TIM_Cmd( TIM5, DISABLE);			// 关闭定时器5
			TIM_ITConfig( TIM5, TIM_IT_Update, DISABLE);
			/* 以下为清除状态 */
			memset(&StepSmallFork,0,sizeof(StepSmallFork));	
			TIM_GPIO_Init();																				/* 初始化GPIO */
			
		/* 清除软件定时器状态 start */
			SmallForkSpeedUp(0) = 0;
			SmallForkSpeedCst(0) = 0;
			SmallForkSpeedDown(0) = 0;
			SmallForkSpeedUp(1) = 0;
			SmallForkSpeedCst(1) = 0;
			SmallForkSpeedDown(1) = 0;			
			SmallForkSpeedUp(3) = 0;
			SmallForkSpeedCst(3) = 0;
			SmallForkSpeedDown(3) = 0;			
		/* 清除软件定时器状态 end */
			break;
		}
		case Rotate_Num:
		{

			TIM_ClearITPendingBit( TIM2, TIM_IT_Update);			// 清除中断标志位
			TIM_CtrlPWMOutputs( TIM8, DISABLE);		// 高级定时器 TIM1 关闭MOE
			TIM_Cmd( TIM8, DISABLE);			// 关闭定时器1
			TIM_Cmd( TIM2, DISABLE);			// 关闭定时器2	
			TIM_ITConfig( TIM2, TIM_IT_Update, DISABLE);
			
			/* 以下为清除状态 */
			memset(&StepRotateFork,0,sizeof(StepRotateFork));	
			TIM_GPIO_Init();																				/* 初始化GPIO */
			
		/* 清除软件定时器状态 start */
			RotateSpeedUp(0) = 0;
			RotateSpeedCst(0) = 0;
			RotateSpeedDown(0) = 0;
			RotateSpeedUp(1) = 0;
			RotateSpeedCst(1) = 0;
			RotateSpeedDown(1) = 0;			
			RotateSpeedUp(3) = 0;
			RotateSpeedCst(3) = 0;
			RotateSpeedDown(3) = 0;			
		/* 清除软件定时器状态 end */		
			break;
		}
	}
}


/****************************END OF FILE****************************/
