#ifndef __UART_H__
#define __UART_H__

#include "device.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

void configure_uart();
int send_uart(char *message, size_t length);
char *receive_uart();

#endif
