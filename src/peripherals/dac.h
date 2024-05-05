#ifndef __DAC_H__
#define __DAC_H__

#include <stdint.h>

void configure_dac(uint16_t *dacBuffer, uint16_t dataLength);
void start_dac();

#endif
