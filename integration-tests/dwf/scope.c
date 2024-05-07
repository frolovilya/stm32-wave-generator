#include "scope.h"
// #include "dwf.h"
#include "device.h"
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>

typedef unsigned char DwfState;
typedef unsigned char STS;
typedef unsigned char TRIGSRC;
typedef int TRIGTYPE;
typedef int DwfTriggerSlope;
typedef int ACQMODE;

static const STS stsDone = 2;

extern int FDwfAnalogInReset(HDWF hdwf);
extern int FDwfDeviceAutoConfigureSet(HDWF hdwf, int fAutoConfigure);
extern int FDwfAnalogInFrequencySet(HDWF hdwf, double hzFrequency);
extern int FDwfAnalogInFrequencyGet(HDWF hdwf, double *phzFrequency);
extern int FDwfAnalogInChannelEnableSet(HDWF hdwf, int idxChannel, int fEnable);
extern int FDwfAnalogInBufferSizeInfo(HDWF hdwf, int *pnSizeMin,
                                      int *pnSizeMax);
extern int FDwfAnalogInBufferSizeSet(HDWF hdwf, int nSize);
extern int FDwfAnalogInChannelRangeSet(HDWF hdwf, int idxChannel,
                                       double voltsRange);
extern int FDwfAnalogInConfigure(HDWF hdwf, int fReconfigure, int fStart);
extern int FDwfAnalogInStatus(HDWF hdwf, int fReadData, DwfState *psts);
extern int FDwfAnalogInStatusData(HDWF hdwf, int idxChannel,
                                  double *rgdVoltData, int cdData);
extern int FDwfAnalogInTriggerSourceSet(HDWF hdwf, TRIGSRC trigsrc);
extern int FDwfAnalogInTriggerAutoTimeoutSet(HDWF hdwf, double secTimeout);
extern int FDwfAnalogInTriggerChannelSet(HDWF hdwf, int idxChannel);
extern int FDwfAnalogInTriggerTypeSet(HDWF hdwf, TRIGTYPE trigtype);
extern int FDwfAnalogInTriggerLevelSet(HDWF hdwf, double voltsLevel);
extern int FDwfAnalogInTriggerConditionSet(HDWF hdwf, DwfTriggerSlope trigcond);
extern int FDwfAnalogInAcquisitionModeSet(HDWF hdwf, ACQMODE acqmode);
extern int FDwfAnalogInTriggerPositionSet(HDWF hdwf, double secPosition);
extern int FDwfAnalogInTriggerPositionGet(HDWF hdwf, double *psecPosition);

#define SCOPE_FREQUENCY 100000
#define SCOPE_CHANNEL 0

static int bufferSize = 1000;

int configure_scope(HDWF device) {
  printf("Configuring Oscilloscope\n");

  FDwfDeviceAutoConfigureSet(device, 0);

  FDwfAnalogInReset(device);

  FDwfAnalogInFrequencySet(device, SCOPE_FREQUENCY);
  FDwfAnalogInChannelEnableSet(device, SCOPE_CHANNEL, 1);

  // set the maximum buffer size
  // FDwfAnalogInBufferSizeInfo(device, NULL, &bufferSize);
  FDwfAnalogInBufferSizeSet(device, bufferSize);

  // set 5V pk2pk input range for all channels
  FDwfAnalogInChannelRangeSet(device, -1, 5.0);

  FDwfAnalogInAcquisitionModeSet(device, 0); // acqmodeSingle

  // configure Auto trigger
  FDwfAnalogInTriggerSourceSet(device, 2);      // trigsrcDetectorAnalogIn
  FDwfAnalogInTriggerAutoTimeoutSet(device, 0); // disable auto trigger
  FDwfAnalogInTriggerChannelSet(device, SCOPE_CHANNEL);
  FDwfAnalogInTriggerTypeSet(device, 0); // trigtypeEdge
  FDwfAnalogInTriggerLevelSet(device, 3);
  FDwfAnalogInTriggerConditionSet(device, 0); // trigcondRisingPositive

  // FDwfAnalogInTriggerPositionSet(device, bufferSize / 2.0 / SCOPE_FREQUENCY);

  sleep(2);

  return 1;
}

int acquire_scope(HDWF device) {
  printf("Starting Oscilloscope data acquisition\n");

  // start
  if (!FDwfAnalogInConfigure(device, 1, 1)) {
    print_last_error("Unable to start Oscilloscope");
    return 0;
  }

  double hzRate;
  FDwfAnalogInFrequencyGet(device, &hzRate);
  printf("Samples: %d, Rate: %d Hz\n", bufferSize, (int)hzRate);

  double triggerPosition;
  FDwfAnalogInTriggerPositionGet(device, &triggerPosition);
  printf("Trigger position: %f\n", triggerPosition);

  // wait for acquisition to be done
  STS sts = 0;
  while (sts != stsDone) {
    FDwfAnalogInStatus(device, 1, &sts);
  }

  // get data
  double samplesBuffer[bufferSize];
  if (!FDwfAnalogInStatusData(device, SCOPE_CHANNEL, samplesBuffer,
                              bufferSize)) {
    print_last_error("Unable to get Oscilloscope data");
    return 0;
  }

  printf("Oscilloscope data acquisition completed\n");

  for (int i = 0; i < bufferSize; i++) {
    printf("%f ", samplesBuffer[i]);
  }
  printf("\n");

  return 1;
}

int measure_frequency() { 
    return 1; 
}