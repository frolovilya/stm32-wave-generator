#ifndef __UART_H__
#define __UART_H__

#include <stddef.h>

void configure_uart();
void send_uart(char *data, size_t dataLength);

#endif
