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
void communication_init(u32 bound);//��ʼ������1���ڸ���λ��ͨ��
void USART1_SendDataString( u8 *pData ,u8 Len);//������������
void Usart_Tx_Num(unsigned char Num);
void Usart_Tx_Char(unsigned char Char);
void Usart_Tx_String(unsigned char *Str);
void rs485_contral_lim(void);																				/* RS485���ƽǱ��� */


/* ������ */
void USART2_Init(u32 bound);
void uartWriteBuf(uint8_t *buf, uint8_t len);
bool isRxCompleted(void);


void USART3_Init(u32 bound);
void Read_Rotate_Angle(void);
void USART3_SendDataString( u8 *pData ,u8 Len);//������������








/* DMA */

//���Ŷ���
/*******************************************************/
#define DEBUG_USART                             USART2

/* ��ͬ�Ĵ��ڹ��ص����߲�һ����ʱ��ʹ�ܺ���Ҳ��һ������ֲʱҪע��
* ����1��6��      RCC_APB2PeriphClockCmd
* ����2/3/4/5��    RCC_APB1PeriphClockCmd
*/

// ���ڶ�Ӧ��DMA����ͨ��
#define  USART_RX_DMA_CHANNEL      DMA_Channel_4
#define  DEBUG_USART_DMA_STREAM    DMA1_Stream5
// ����Ĵ�����ַ
#define  USART_DR_ADDRESS        (&DEBUG_USART->DR)
// һ�η��͵�������
#define  USART_RBUFF_SIZE            1000 



#define DEBUG_USART_CLK                         RCC_APB1Periph_USART2
#define DEBUG_USART_BAUDRATE                    115200  //���ڲ�����

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













