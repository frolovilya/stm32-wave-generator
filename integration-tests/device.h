#ifndef __DEVICE_H__
#define __DEVICE_H__

#include <stdio.h>

typedef int HDWF;

HDWF open_device();
void close_device(HDWF device);
void print_last_error(char *prefix);

#endif
