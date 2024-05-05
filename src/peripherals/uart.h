#ifndef __UART_H__
#define __UART_H__

#include <stddef.h>
#include <stm32f446xx.h>

typedef void (*uart_rx_handler_t)(char *str);

void configure_uart(USART_TypeDef *uartHandler);
void start_uart();
void send_uart(char *data, size_t dataLength);
void receive_uart(uart_rx_handler_t callback);

#endif
