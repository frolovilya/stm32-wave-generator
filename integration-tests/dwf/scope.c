#include "scope.h"
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
typedef int DwfWindow;

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
extern int FDwfSpectrumWindow(double *rgdWin, int cdWin, DwfWindow iWindow,
                              const double vBeta, double *vNEBW);
extern int FDwfSpectrumFFT(const double *rgdData, int cdData, double *rgdBin,
                           double *rgdPhase, int cdBin);

#define SCOPE_FREQUENCY 100000
#define SCOPE_CHANNEL 0

#define SAMPLE_BUFFER_SIZE 16384
static double samplesBuffer[SAMPLE_BUFFER_SIZE];

int configure_scope(HDWF device) {
  printf("Configuring Oscilloscope\n");

  FDwfDeviceAutoConfigureSet(device, 0);

  FDwfAnalogInReset(device);

  FDwfAnalogInFrequencySet(device, SCOPE_FREQUENCY);
  FDwfAnalogInChannelEnableSet(device, SCOPE_CHANNEL, 1);

  // set the buffer size
  // int maxBufferSize = 0;
  // FDwfAnalogInBufferSizeInfo(device, NULL, &maxBufferSize);
  // printf("Max buffer size: %d\n", maxBufferSize);
  FDwfAnalogInBufferSizeSet(device, SAMPLE_BUFFER_SIZE);

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

int capture_samples(HDWF device) {
  // start
  if (!FDwfAnalogInConfigure(device, 1, 1)) {
    print_last_error("Unable to start Oscilloscope");
    return 0;
  }

  double hzRate;
  FDwfAnalogInFrequencyGet(device, &hzRate);
  printf("Capturing %d samples at %d Hz\n", SAMPLE_BUFFER_SIZE, (int)hzRate);

  // double triggerPosition;
  // FDwfAnalogInTriggerPositionGet(device, &triggerPosition);
  // printf("Trigger position: %f\n", triggerPosition);

  // wait for acquisition to be done
  STS sts = 0;
  while (sts != stsDone) {
    FDwfAnalogInStatus(device, 1, &sts);
  }

  // get data
  if (!FDwfAnalogInStatusData(device, SCOPE_CHANNEL, samplesBuffer,
                              SAMPLE_BUFFER_SIZE)) {
    print_last_error("Unable to get Oscilloscope data");
    return 0;
  }

  printf("Oscilloscope data capturing completed\n");

  return 1;
}

double measure_samples_frequency(HDWF device) {
  double windowBuffer[SAMPLE_BUFFER_SIZE];
  // DwfWindowFlatTop = 9
  FDwfSpectrumWindow(windowBuffer, SAMPLE_BUFFER_SIZE, 9, 1, NULL);
  for (int i = 0; i < SAMPLE_BUFFER_SIZE; i++) {
    samplesBuffer[i] *= windowBuffer[i];
  }

  int binsCount = SAMPLE_BUFFER_SIZE / 2 + 1;
  double fBins[binsCount];
  FDwfSpectrumFFT(samplesBuffer, SAMPLE_BUFFER_SIZE, fBins, NULL, binsCount);

  int maxBinIndex = 0;
  double maxBinValue = 0;
  for (int i = 5; i < binsCount; i++) {
    if (fBins[i] > maxBinValue) {
      maxBinValue = fBins[i];
      maxBinIndex = i;
    }
  }

  double hzRate;
  FDwfAnalogInFrequencyGet(device, &hzRate);

  double maxFrequency = (hzRate / 2 * maxBinIndex) / (binsCount - 1);
  printf("Frequency: %f\n", maxFrequency);

  return maxFrequency;

  /*printf("Bins:\n");
  for (int i = 0; i < binsCount; i++) {
    printf("%f ", fBins[i]);
  }
  printf("\n");

  printf("Samples:\n");
  for (int i = 0; i < SAMPLE_BUFFER_SIZE; i++) {
    printf("%f ", samplesBuffer[i]);
  }
  printf("\n");*/
}