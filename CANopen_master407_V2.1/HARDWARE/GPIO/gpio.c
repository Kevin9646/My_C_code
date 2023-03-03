#include "gpio.h" 


//初始化PF9和PF10为输出口.并使能这两个口的时钟		    
//LED IO初始化
void gpio_Init(void)
{    	 
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOD, ENABLE);//使能GPIOC时钟

	
  //步进电机使能及方向控制角初始化


  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	
	
	/* 步进电机功能角配置 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11;//PC9-2ENA/PC10-1ENA/PC8-2DIR/PC11-1DIR
  GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化GPIO
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14;//PD13-3DIR/PD14-3ENA
	GPIO_Init(GPIOD, &GPIO_InitStructure);//初始化GPIO

	/* RS485功能角配置 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//RS485_control
	GPIO_Init(GPIOD, &GPIO_InitStructure);//初始化GPIO	
	
	/* 舵机 TXEN_A1  RXEN_A0 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;//RS485_control
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIO	
	
	
	
	GPIO_ResetBits(GPIOC,GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11);
	GPIO_ResetBits(GPIOD,GPIO_Pin_13|GPIO_Pin_14);
	GPIO_ResetBits(GPIOA,GPIO_Pin_8|GPIO_Pin_0|GPIO_Pin_1);
	
	
	
	
	
	
	
	
}






