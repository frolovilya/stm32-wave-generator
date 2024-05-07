#include "uart.h"

extern int FDwfDigitalUartReset(HDWF hdwf);
extern int FDwfDigitalUartRateSet(HDWF hdwf, double hz);
extern int FDwfDigitalUartBitsSet(HDWF hdwf, int cBits);
extern int FDwfDigitalUartParitySet(HDWF hdwf, int parity);
extern int FDwfDigitalUartPolaritySet(HDWF hdwf, int polarity);
extern int FDwfDigitalUartStopSet(HDWF hdwf, double cBit);
extern int FDwfDigitalUartTxSet(HDWF hdwf, int idxChannel);
extern int FDwfDigitalUartRxSet(HDWF hdwf, int idxChannel);

extern int FDwfDigitalUartTx(HDWF hdwf, char *szTx, int cTx);
extern int FDwfDigitalUartRx(HDWF hdwf, char *szRx, int cRx, int *pcRx,
                             int *pParity);

#define BAUD_RATE 115200
#define TX_PIN 0
#define RX_PIN 1

struct timespec tim, tim2;

static void sleep_ms() {
  // sleep for 500ms
  tim.tv_sec = 0;
  tim.tv_nsec = 500000000L;

  nanosleep(&tim, &tim2);
}

/**
 * Configure UART protocol for transmitting and receiving data
 * 
 * @param device DWF device handler
*/
void configure_uart(HDWF device) {
  printf("Configuring UART\n");

  FDwfDigitalUartRateSet(device, BAUD_RATE);
  FDwfDigitalUartBitsSet(device, 8);
  FDwfDigitalUartParitySet(device, 0);
  FDwfDigitalUartStopSet(device, 1);
  FDwfDigitalUartTxSet(device, TX_PIN);
  FDwfDigitalUartRxSet(device, RX_PIN);

  int responseSize = 0;
  int responseParity = 0;
  char *dummy_buffer = NULL;

  // initialize TX, drive with idle level
  FDwfDigitalUartTx(device, dummy_buffer, 0);
  // initialize RX reception
  FDwfDigitalUartRx(device, dummy_buffer, 0, &responseSize, &responseParity);

  sleep_ms();
}

/**
 * Send a message via UART protocol
 * 
 * @param device DWF device handler
 * @param message data to send 
 * @param length data length
 * @return 1 if successfully send, 0 otherwise
*/
int send_uart(HDWF device, char *message, size_t length) {
  char buffer[length + 1];
  strncpy(buffer, message, length);
  buffer[length] = '\n';

  if (!FDwfDigitalUartTx(device, buffer, length + 1)) {
    print_last_error("Error sending UART message");
    return 0;
  }
  // printf("Sent: %s\n", message);

  sleep_ms();

  return 1;
}

/**
 * Receive a message from UART
 * 
 * @param device DWF device handler
 * @return received data buffer (must be freed after usage)
*/
char *receive_uart(HDWF device) {
  char buffer[100];
  int responseSize = 0;
  int responseParity = 0;
  while (responseSize == 0) {
    if (!FDwfDigitalUartRx(device, buffer, 100, &responseSize,
                           &responseParity)) {
      print_last_error("Error receiving UART data");
      return NULL;
    }
  }

  char receivedString[responseSize];
  strncpy(receivedString, buffer, responseSize - 1);
  receivedString[responseSize - 1] = '\0';

  // printf("Received: %s\n", receivedString);

  return strdup(receivedString);
}