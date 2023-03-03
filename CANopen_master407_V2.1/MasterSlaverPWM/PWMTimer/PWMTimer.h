#ifndef __TIMER_H
#define __TIMER_H

#include "main.h"
#include "sys.h"

/* Group A: TIM8 - 主定时器，TIM2 - 从定时器
	 TIM8: CH1 - PC7, 				   RCC_APB2Periph_TIM8, RCC_AHB1Periph_GPIOC
   TIM2: TIM2: RCC_APB1Periph_TIM2
	 
	 Group B: TIM4 - 主定时器，TIM1 - 从定时器
	 TIM4: CH1 - PD12,  				RCC_APB1Periph_TIM4  RCC_AHB1Periph_GPIOD
   TIM8: RCC_APB2Periph_TIM1
	 
	 Group C: TIM3 - 主定时器，TIM5 - 从定时器
	 TIM3: TIM3: CH2 - PC6					RCC_APB1Periph_TIM3  RCC_AHB1Periph_GPIOC
   TIM5: RCC_APB1Periph_TIM5	
	 
分别调用
Group1	 void PWM_Output_A(u16 Cycle_A, u32 PulseNum_A);
Group2	 void PWM_Output_B(u16 Cycle_A, u32 PulseNum_A); 
Group3	 void PWM_Output_C(u16 Cycle_A, u32 PulseNum_A); 
	 
指定输出脉冲	 
分频系数为168m和84M
1s100个脉冲
	 
*/


/*****定时器初始化函数*****/
// Group A
void TIM8_GPIO_Config(u16 TIM8_Prescaler, u16 TIM8_Period, u16 CCR_A);
void TIM2_GPIO_Config(u32 PulseNum_A);
void PWM_Output_A(u16 Cycle_A, u32 PulseNum_A);
void TIM2_IRQHandler(void);

// Group B
void TIM4_GPIO_Config(u16 TIM4_Prescaler, u16 TIM4_Period, u16 CCR_B);
void TIM1_GPIO_Config(u32 PulseNum_B);
void PWM_Output_B(u16 Cycle_B, u32 PulseNum_B);
void TIM1_UP_TIM10_IRQHandler(void);

// Group C
void TIM3_GPIO_Config(u16 TIM3_Prescaler, u16 TIM3_Period, u16 CCR_C);
void TIM5_GPIO_Config(u32 PulseNum_C);
void PWM_Output_C(u16 Cycle_C, u32 PulseNum_C);
void TIM5_IRQHandler(void);



/* 停止步进电机 */
void CloseSteptMotor(u8 steptMotor_num);
void	TIM_GPIO_Init(void);


#endif	/* __TIMER_H */

/****************************END OF FILE****************************/
