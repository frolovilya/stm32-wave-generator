#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "dwf/device.h"
#include "dwf/uart.h"
#include "dwf/scope.h"

static void test_command(HDWF device, char *request, char *expectedResponse) {
    if (!send_uart(device, request, strlen(request))) {
        return;
    }

    char *response;
    if ((response = receive_uart(device)) == NULL) {
        return;
    }

    printf("request=%s, expected response=%s, actual response=%s\n\t",
        request, expectedResponse, response);

    if(strcmp(response, expectedResponse) == 0) {
        printf("(+) passed\n");
    } else {
        printf("(-) failed!\n");
    }

    free(response);
}

int main() {
    HDWF device;
    if ((device = open_device()) < 0) {
        return 1;
    }

    /*configure_uart(device);

    test_command(device, "-440", "Freq: 20");
    test_command(device, "10", "Freq: 20");
    test_command(device, "123", "Freq: 123");
    test_command(device, "1000", "Freq: 1000");
    test_command(device, "100000", "Freq: 20");*/

    configure_scope(device);
    acquire_scope(device);

    close_device(device);

    return 0;
}