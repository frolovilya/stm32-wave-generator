#include "dwf/device.h"
#include "dwf/scope.h"
#include "dwf/uart.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static void test_command(HDWF device, char *request, char *expectedResponse) {
  if (!send_uart(device, request, strlen(request))) {
    return;
  }

  char *response;
  if ((response = receive_uart(device)) == NULL) {
    return;
  }

  printf("request=%s, expected response=%s, actual response=%s\n\t", request,
         expectedResponse, response);

  if (strcmp(response, expectedResponse) == 0) {
    printf("(+) passed\n");
  } else {
    printf("(-) failed!\n");
  }

  free(response);
}

static void test_wave_frequency(HDWF device, int expectedFrequency) {
  char request[100];
  int requestLength = sprintf(request, "%d", expectedFrequency);
  if (!send_uart(device, request, requestLength)) {
    return;
  }

  capture_samples(device);
  double actualFrequency = measure_samples_frequency(device);

  double frequencyDiff = fabs(actualFrequency - expectedFrequency);
  double diffPercent = frequencyDiff / expectedFrequency * 100;

  printf("expected frequency=%d, actual frequency=%f, diff=%f (%f%%)",
         expectedFrequency, actualFrequency, frequencyDiff, diffPercent);
}

int main() {
  HDWF device;
  if ((device = open_device()) < 0) {
    return 1;
  }

  configure_uart(device);
  configure_scope(device);

  /*test_command(device, "-440", "Freq: 20");
  test_command(device, "10", "Freq: 20");
  test_command(device, "123", "Freq: 123");
  test_command(device, "1000", "Freq: 1000");
  test_command(device, "100000", "Freq: 20");*/

  test_wave_frequency(device, 200);
  test_wave_frequency(device, 440);
  test_wave_frequency(device, 2000);
  test_wave_frequency(device, 5000);
  // test_wave_frequency(device, 10000);
  // test_wave_frequency(device, 19000);

  close_device(device);

  return 0;
}