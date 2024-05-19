#ifndef __UART_H
#define __UART_H	 
#include "sys.h"

extern uint8_t uart_dat;

void USART1_Init(int Buad);
void USART_SendByte(USART_TypeDef *USARTx,char str);
void USART_SendString(USART_TypeDef *USARTx,char *str);
    
#endif
