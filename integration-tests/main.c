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
  test_uart_command("help", "Usage: sine|square|saw|triangle 20..20000");
  test_uart_command("sine 10", "Generating 20Hz sine wave");
  test_uart_command("sine 220", "Generating 220Hz sine wave");
  test_uart_command("sine 100000", "Generating 20000Hz sine wave");
  test_uart_command("saw 440", "Generating 440Hz saw wave");
  test_uart_command("square 1000", "Generating 1000Hz square wave");
  test_uart_command("triangle 15000", "Generating 15000Hz triangle wave");

  printf("\nTesting wave generation (tolerance=%d%%)\n",
         WAVE_FREQUENCY_TOLERANCE_PERCENTS);
  test_wave_frequency("sine", 200);
  test_wave_frequency("sine", 440);
  test_wave_frequency("sine", 2000);
  test_wave_frequency("sine", 5000);

  test_wave_frequency("square", 200);
  test_wave_frequency("square", 440);
  test_wave_frequency("square", 2000);
  test_wave_frequency("square", 5000);

  test_wave_frequency("saw", 200);
  test_wave_frequency("saw", 440);
  test_wave_frequency("saw", 2000);
  test_wave_frequency("saw", 5000);

  test_wave_frequency("triangle", 200);
  test_wave_frequency("triangle", 440);
  test_wave_frequency("triangle", 2000);
  test_wave_frequency("triangle", 5000);
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