#ifndef __SCOPE_H__
#define __SCOPE_H__

#include "device.h"

int configure_scope(HDWF device);
int capture_samples(HDWF device);
double measure_samples_frequency(HDWF device);

#endif
