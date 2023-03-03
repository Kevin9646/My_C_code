#ifndef __USART_H
#define __USART_H


#include "sys.h" 
#include "main.h"	



//#define RS232_USART                             USART2
//#define RS232_USART_CLK                         RCC_APB1Periph_USART2

//#define RS232_USART_RX_GPIO_PORT                GPIOA
//#define RS232_USART_RX_GPIO_CLK                 RCC_AHB1Periph_GPIOA
//#define RS232_USART_RX_PIN                      GPIO_Pin_3
//#define RS232_USART_RX_AF                       GPIO_AF_USART2
//#define RS232_USART_RX_SOURCE                   GPIO_PinSource3

//#define RS232_USART_TX_GPIO_PORT                GPIOA
//#define RS232_USART_TX_GPIO_CLK                 RCC_AHB1Periph_GPIOA
//#define RS232_USART_TX_PIN                      GPIO_Pin_2
//#define RS232_USART_TX_AF                       GPIO_AF_USART2
//#define RS232_USART_TX_SOURCE                   GPIO_PinSource2


//#define RS232_USART_IRQHandler                   USART2_IRQHandler
//#define RS232_USART_IRQ                 					USART2_IRQn


void USART1_Init(u32 bound);
void communication_init(u32 bound);//初始化串口1用于跟上位机通信
void USART1_SendDataString( u8 *pData ,u8 Len);//连续发送数据
void Usart_Tx_Num(unsigned char Num);
void Usart_Tx_Char(unsigned char Char);
void Usart_Tx_String(unsigned char *Str);
void rs485_contral_lim(void);																				/* RS485控制角保护 */


/* 舵机相关 */
void USART2_Init(u32 bound);
void uartWriteBuf(uint8_t *buf, uint8_t len);
bool isRxCompleted(void);


void USART3_Init(u32 bound);
void Read_Rotate_Angle(void);
void USART3_SendDataString( u8 *pData ,u8 Len);//连续发送数据








/* DMA */

//引脚定义
/*******************************************************/
#define DEBUG_USART                             USART2

/* 不同的串口挂载的总线不一样，时钟使能函数也不一样，移植时要注意
* 串口1和6是      RCC_APB2PeriphClockCmd
* 串口2/3/4/5是    RCC_APB1PeriphClockCmd
*/

// 串口对应的DMA请求通道
#define  USART_RX_DMA_CHANNEL      DMA_Channel_4
#define  DEBUG_USART_DMA_STREAM    DMA1_Stream5
// 外设寄存器地址
#define  USART_DR_ADDRESS        (&DEBUG_USART->DR)
// 一次发送的数据量
#define  USART_RBUFF_SIZE            1000 



#define DEBUG_USART_CLK                         RCC_APB1Periph_USART2
#define DEBUG_USART_BAUDRATE                    115200  //串口波特率

#define DEBUG_USART_RX_GPIO_PORT                GPIOA
#define DEBUG_USART_RX_GPIO_CLK                 RCC_AHB1Periph_GPIOA
#define DEBUG_USART_RX_PIN                      GPIO_Pin_3
#define DEBUG_USART_RX_AF                       GPIO_AF_USART2
#define DEBUG_USART_RX_SOURCE                   GPIO_PinSource3

#define DEBUG_USART_TX_GPIO_PORT                GPIOA
#define DEBUG_USART_TX_GPIO_CLK                 RCC_AHB1Periph_GPIOA
#define DEBUG_USART_TX_PIN                      GPIO_Pin_2
#define DEBUG_USART_TX_AF                       GPIO_AF_USART2
#define DEBUG_USART_TX_SOURCE                   GPIO_PinSource2

#define DEBUG_USART_IRQHandler                  USART2_IRQHandler
#define DEBUG_USART_IRQ                 				USART2_IRQn
/************************************************************/

void Debug_USART_Config(void);
void USARTx_DMA_Config(void);
void Uart_DMA_Rx_Data(void);

void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch);
void Usart_SendString( USART_TypeDef * pUSARTx, char *str);

void Usart_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch);









#endif 













