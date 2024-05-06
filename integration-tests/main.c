#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "device.h"
#include "uart.h"

int main() {
    HDWF device;
    if (!(device = open_device())) {
        return 1;
    }

    configure_uart(device);

    char *request = "555";
    if (!send_uart(device, request, strlen(request))) {
        return 1;
    }

    char *response = receive_uart(device);
    free(response);

    close_device(device);

    return 0;
}