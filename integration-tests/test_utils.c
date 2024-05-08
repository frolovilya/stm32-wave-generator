#include "test_utils.h"

/**
 * Test UART command response
 *
 * @param request request string to be sent via UART
 * @param expectedResponse expected command response
 */
void test_uart_command(char *request, char *expectedResponse) {
  if (!send_uart(request, strlen(request))) {
    return;
  }

  char *response;
  if ((response = receive_uart()) == NULL) {
    return;
  }

  printf("request=%s, expected response=%s, actual response=%s\n", request,
         expectedResponse, response);

  if (strcmp(response, expectedResponse) == 0) {
    printf("\t(+) passed\n");
  } else {
    printf("\t(-) failed!\n");
  }
  printf("-----\n");

  free(response);
}

/**
 * Test generated wave frequency
 *
 * @param waveForm target wave form (sine|square|saw|triangle)
 * @param expectedFrequency expected frequency is passed to the generator as an
 * input and compared with the actual result
 */
void test_wave_frequency(char *waveForm, int expectedFrequency) {
  char request[100];
  int requestLength = sprintf(request, "%s %d", waveForm, expectedFrequency);
  if (!send_uart(request, requestLength)) {
    return;
  }

  capture_samples();
  double actualFrequency = measure_frequency();

  double frequencyDiff = fabs(actualFrequency - expectedFrequency);
  double diffPercent = frequencyDiff / expectedFrequency * 100;

  printf(
      "wave=%s, expected frequency=%d, actual frequency=%f, diff=%f (%f%%)\n",
      waveForm, expectedFrequency, actualFrequency, frequencyDiff, diffPercent);

  if (diffPercent) {
    printf("\t(+) passed\n");
  } else {
    printf("\t(-) failed!\n");
  }
  printf("-----\n");
}