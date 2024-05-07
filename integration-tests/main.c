#include "dwf/device.h"
#include "dwf/scope.h"
#include "dwf/uart.h"
#include "test_utils.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static void test_suite() {
  printf("Testing UART controls\n");
  test_uart_command("-440", "Freq: 20");
  test_uart_command("10", "Freq: 20");
  test_uart_command("123", "Freq: 123");
  test_uart_command("1000", "Freq: 1000");
  test_uart_command("100000", "Freq: 20");

  printf("\nTesting wave generation (tolerance=%d%%)\n",
         WAVE_FREQUENCY_TOLERANCE_PERCENTS);
  test_wave_frequency(200);
  test_wave_frequency(440);
  test_wave_frequency(2000);
  test_wave_frequency(5000);
}

int main() {
  HDWF device;
  if ((device = open_device()) < 0) {
    return 1;
  }

  configure_uart();
  configure_scope();

  test_suite();

  close_device();

  return 0;
}