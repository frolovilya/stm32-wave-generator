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
extern int FDwfSpectrumWindow(double *rgdWin, int cdWin, DwfWindow iWindow,
                              const double vBeta, double *vNEBW);
extern int FDwfSpectrumFFT(const double *rgdData, int cdData, double *rgdBin,
                           double *rgdPhase, int cdBin);

// Wave generation output sampling frequency is 48KHz
// Capturing the signal at 100KHz
#define SCOPE_FREQUENCY 100000

#ifndef SCOPE_CHANNEL
// Using Channel 1 (index 0) as a default
#define SCOPE_CHANNEL 1
#endif
#define SCOPE_CHANNEL_INDEX SCOPE_CHANNEL-1

#define SAMPLE_BUFFER_SIZE 16384
static double samplesBuffer[SAMPLE_BUFFER_SIZE];

/**
 * Configure Oscilloscope to capture wave data
 */
void configure_scope() {
  printf("Configuring Oscilloscope (channel %d)\n", SCOPE_CHANNEL);

  FDwfDeviceAutoConfigureSet(get_device(), 0);

  FDwfAnalogInReset(get_device());

  FDwfAnalogInFrequencySet(get_device(), SCOPE_FREQUENCY);
  FDwfAnalogInChannelEnableSet(get_device(), SCOPE_CHANNEL_INDEX, 1);

  // set the buffer size
  // int maxBufferSize = 0;
  // FDwfAnalogInBufferSizeInfo(get_device(), NULL, &maxBufferSize);
  // printf("Max buffer size: %d\n", maxBufferSize);
  FDwfAnalogInBufferSizeSet(get_device(), SAMPLE_BUFFER_SIZE);

  // set 5V pk2pk input range for all channels
  FDwfAnalogInChannelRangeSet(get_device(), -1, 5.0);

  FDwfAnalogInAcquisitionModeSet(get_device(), 0); // acqmodeSingle

  // configure Auto trigger
  FDwfAnalogInTriggerSourceSet(get_device(), 2);      // trigsrcDetectorAnalogIn
  FDwfAnalogInTriggerAutoTimeoutSet(get_device(), 0); // disable auto trigger
  FDwfAnalogInTriggerChannelSet(get_device(), SCOPE_CHANNEL_INDEX);
  FDwfAnalogInTriggerTypeSet(get_device(), 0); // trigtypeEdge
  FDwfAnalogInTriggerLevelSet(get_device(), 3);
  FDwfAnalogInTriggerConditionSet(get_device(), 0); // trigcondRisingPositive

  sleep(2);
}

/**
 * Capture samples into internal buffer
 *
 * @return 1 if successful and 0 otherwise
 */
int capture_samples() {
  // start
  if (!FDwfAnalogInConfigure(get_device(), 1, 1)) {
    print_last_error("Unable to start Oscilloscope");
    return 0;
  }

  double hzRate;
  FDwfAnalogInFrequencyGet(get_device(), &hzRate);
  printf("Capturing %d samples at %d Hz\n", SAMPLE_BUFFER_SIZE, (int)hzRate);

  // wait for acquisition to be done
  STS sts = 0;
  while (sts != stsDone) {
    FDwfAnalogInStatus(get_device(), 1, &sts);
  }

  // get data
  if (!FDwfAnalogInStatusData(get_device(), SCOPE_CHANNEL_INDEX, samplesBuffer,
                              SAMPLE_BUFFER_SIZE)) {
    print_last_error("Unable to get Oscilloscope data");
    return 0;
  }

  printf("Oscilloscope data capturing completed\n");

  return 1;
}

/**
 * Measure frequency of a sampled signal in internal buffer
 *
 * @return signal frequency
 */
double measure_frequency() {
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
  FDwfAnalogInFrequencyGet(get_device(), &hzRate);

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