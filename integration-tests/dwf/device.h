#ifndef __DEVICE_H__
#define __DEVICE_H__

#include <stdio.h>

typedef int HDWF;

HDWF get_device();
int open_device();
void close_device();
void print_last_error(char *prefix);

#endif
