#ifndef __UART_H__
#define __UART_H__

#include <stddef.h>
#include <stm32f446xx.h>

void configure_uart(USART_TypeDef *uartHandler);
void start_uart();
void send_uart(char *data, size_t dataLength);

#endif
