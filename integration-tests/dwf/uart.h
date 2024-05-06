#ifndef __UART_H__
#define __UART_H__

#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>
#include <time.h>
#include "device.h"

void configure_uart(HDWF device);
int send_uart(HDWF device, char *message, size_t length);
char *receive_uart(HDWF device);

#endif
