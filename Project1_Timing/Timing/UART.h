#ifndef __STM32L476G_DISCOVERY_UART_H
#define __STM32L476G_DISCOVERY_UART_H

#include "stm32l476xx.h"

#define BufferSize 	(32)
#define BUFFER_SIZE (256)

void UART2_Init(void);
void UART2_GPIO_Init(void);
void USART1_IRQHandler(void);
void USART2_IRQHandler(void);
void USART_Init(USART_TypeDef * USARTx);
void USART_Write(USART_TypeDef * USARTx, uint8_t *buffer);
void USART_WriteUInt8(USART_TypeDef * USARTx, uint8_t num);
void USART_WriteUInt16(USART_TypeDef * USARTx, uint16_t num);
uint8_t   USART_Read(USART_TypeDef * USARTx);
uint8_t*	USART_ReadStr(USART_TypeDef * USARTx);

void USART_Delay(uint32_t us);
void USART_IRQHandler(USART_TypeDef * USARTx, uint8_t *buffer, uint32_t * pRx_counter);

#endif /* __STM32L476G_DISCOVERY_UART_H */
