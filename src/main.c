#include <stm32f446xx.h>
#include <string.h>
#include <stdio.h>
#include "rcc.h"
#include "timer.h"
#include "uart.h"

int main()
{
    configure_clocks();
    //configure_timer();
    configure_uart();

    char *msg = "Hello, world!\n";
    //send_uart(msg, strlen(msg));
    printf(msg);

    while(1) {
        //send_uart(msg, strlen(msg));
    }

    return 0;

}