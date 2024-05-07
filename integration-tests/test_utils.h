#ifndef __TEST_H__
#define __TEST_H__

#include "dwf/device.h"
#include "dwf/scope.h"
#include "dwf/uart.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define WAVE_FREQUENCY_TOLERANCE_PERCENTS 5

void test_uart_command(char *request, char *expectedResponse);
void test_wave_frequency(int expectedFrequency);

#endif
