#ifndef __STM32L476G_DISCOVERY_UART_H
#define __STM32L476G_DISCOVERY_UART_H

#include "stm32l476xx.h"

#define BufferSize 	(32)
#define BUFFER_SIZE (256)

#define PRINT(x)					USART_Write(USART2, (uint8_t*)x)
//#define PRINT_UINT16(x)		USART_WriteUInt16(USART2, x);
#define PRINT_UINT16(x)		USART_WriteUInt32(USART2, (uint32_t) x);
#define PRINT_UINT32(x)		USART_WriteUInt32(USART2, x);

void UART2_Init(void);
void UART2_GPIO_Init(void);
void USART1_IRQHandler(void);
void USART2_IRQHandler(void);
void USART_Init(USART_TypeDef * USARTx);
void USART_Write(USART_TypeDef * USARTx, uint8_t *buffer);
void USART_WriteUInt8(USART_TypeDef * USARTx, uint8_t num);
//void USART_WriteUInt16(USART_TypeDef * USARTx, uint16_t num);
void USART_WriteUInt32(USART_TypeDef * USARTx, uint32_t num);
uint8_t PromptYesOrNo(uint8_t* prompt);
uint8_t   USART_Read(USART_TypeDef * USARTx);
uint8_t*	USART_ReadStr(USART_TypeDef * USARTx);

void USART_Delay(uint32_t us);
void USART_IRQHandler(USART_TypeDef * USARTx, uint8_t *buffer, uint32_t * pRx_counter);

#endif /* __STM32L476G_DISCOVERY_UART_H */
