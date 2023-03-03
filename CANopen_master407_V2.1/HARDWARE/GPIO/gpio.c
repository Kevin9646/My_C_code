#include "gpio.h" 


//��ʼ��PF9��PF10Ϊ�����.��ʹ���������ڵ�ʱ��		    
//LED IO��ʼ��
void gpio_Init(void)
{    	 
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOD, ENABLE);//ʹ��GPIOCʱ��

	
  //�������ʹ�ܼ�������ƽǳ�ʼ��


  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	
	
	/* ����������ܽ����� */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11;//PC9-2ENA/PC10-1ENA/PC8-2DIR/PC11-1DIR
  GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��GPIO
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14;//PD13-3DIR/PD14-3ENA
	GPIO_Init(GPIOD, &GPIO_InitStructure);//��ʼ��GPIO

	/* RS485���ܽ����� */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//RS485_control
	GPIO_Init(GPIOD, &GPIO_InitStructure);//��ʼ��GPIO	
	
	/* ��� TXEN_A1  RXEN_A0 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;//RS485_control
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIO	
	
	
	
	GPIO_ResetBits(GPIOC,GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11);
	GPIO_ResetBits(GPIOD,GPIO_Pin_13|GPIO_Pin_14);
	GPIO_ResetBits(GPIOA,GPIO_Pin_8|GPIO_Pin_0|GPIO_Pin_1);
	
	
	
	
	
	
	
	
}






