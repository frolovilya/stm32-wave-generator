#include "uart.h"

extern int FDwfDigitalUartRateSet();
extern int FDwfDigitalUartBitsSet();
extern int FDwfDigitalUartParitySet();
extern int FDwfDigitalUartStopSet();
extern int FDwfDigitalUartTxSet();
extern int FDwfDigitalUartRxSet();
extern int FDwfDigitalUartRx();
extern int FDwfDigitalUartTx();

void configure_uart(HDWF device) {
    printf("Configuring UART\n");

    FDwfDigitalUartRateSet(device, 115200.0);
    FDwfDigitalUartBitsSet(device, 8);
    FDwfDigitalUartParitySet(device, 0);
    FDwfDigitalUartStopSet(device, 1);
    FDwfDigitalUartTxSet(device, 0);
    FDwfDigitalUartRxSet(device, 1);

    int responseSize = 0;
    int responseParity = 0;
    char* dummy_buffer = NULL;

    // initialize RX reception
    FDwfDigitalUartRx(device, dummy_buffer, 0, &responseSize, &responseParity);
    // initialize TX, drive with idle level
    FDwfDigitalUartTx(device, dummy_buffer, 0);
}

int send_uart(HDWF device, char *message, size_t length) {
    char buffer[length + 1];
    strncpy(buffer, message, length);
    buffer[length] = '\n';

    if(!FDwfDigitalUartTx(device, buffer, length + 1)) {
        print_last_error("Error sending UART message");
        return 0;
    }
    printf("Sent: %s\n", message);
    return 1;
}

char *receive_uart(HDWF device) {
    char buffer[100];
    int responseSize = 0;
    int responseParity = 0;
    while(responseSize == 0) {
        if(!FDwfDigitalUartRx(device, &buffer[0], 100, &responseSize, &responseParity)) {
            print_last_error("Error receiving UART data");
            return NULL;
        }
    }
    char receivedString[responseSize];
    strncpy(receivedString, buffer, responseSize - 1);
    receivedString[responseSize - 1] = '\0';

    printf("Received: %s\n", receivedString);

    return strdup(receivedString);
}