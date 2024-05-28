#include "Oscilloscope.hpp"
#include "dwf.h"
#include <iostream>
#include <stdexcept>
#include <chrono>
#include <thread>

using namespace std;

Oscilloscope::Oscilloscope(const DwfDeviceHandler &deviceHandler, int frequency,
                           int channel, int sampleBufferSize)
    : deviceHandler{deviceHandler}, frequency{frequency},
      channelIndex{channel - 1}, sampleBufferSize{sampleBufferSize} {
  configure();
}

/**
 * Configure Oscilloscope to capture wave data
 */
void Oscilloscope::configure() {
  cout << "Configuring Oscilloscope (channel " << (channelIndex + 1) << ")\n";

  const HDWF device = deviceHandler.getDevice();

  FDwfDeviceAutoConfigureSet(device, 0);

  FDwfAnalogInReset(device);

  FDwfAnalogInFrequencySet(device, frequency);
  FDwfAnalogInChannelEnableSet(device, channelIndex, 1);

  // set the buffer size
  // int maxBufferSize = 0;
  // FDwfAnalogInBufferSizeInfo(get_device(), NULL, &maxBufferSize);
  // printf("Max buffer size: %d\n", maxBufferSize);
  FDwfAnalogInBufferSizeSet(device, sampleBufferSize);

  // set 5V pk2pk input range for all channels
  FDwfAnalogInChannelRangeSet(device, -1, 5.0);

  FDwfAnalogInAcquisitionModeSet(device, 0); // acqmodeSingle

  // configure Auto trigger
  FDwfAnalogInTriggerSourceSet(device, 2);      // trigsrcDetectorAnalogIn
  FDwfAnalogInTriggerAutoTimeoutSet(device, 0); // disable auto trigger
  FDwfAnalogInTriggerChannelSet(device, channelIndex);
  FDwfAnalogInTriggerTypeSet(device, 0); // trigtypeEdge
  FDwfAnalogInTriggerLevelSet(device, 3);
  FDwfAnalogInTriggerConditionSet(device, 0); // trigcondRisingPositive

  std::this_thread::sleep_for(std::chrono::seconds(2));
}

/**
 * Capture samples into internal buffer
 */
vector<double> Oscilloscope::captureSamples() const {
  const HDWF device = deviceHandler.getDevice();
  double samplesBuffer[sampleBufferSize];

  // start
  if (!FDwfAnalogInConfigure(device, 1, 1)) {
    throw runtime_error(deviceHandler.getLastError("Unable to start Oscilloscope"));
  }

  double hzRate;
  FDwfAnalogInFrequencyGet(device, &hzRate);
  cout << "Capturing " << sampleBufferSize << " samples at " << (int)hzRate << "Hz\n";

  // wait for acquisition to be done
  STS sts = 0;
  while (sts != stsDone) {
    FDwfAnalogInStatus(device, 1, &sts);
  }

  // get data
  if (!FDwfAnalogInStatusData(device, channelIndex, samplesBuffer,
                              sampleBufferSize)) {
    throw runtime_error(deviceHandler.getLastError("Unable to get Oscilloscope data"));
  }

  cout << "Oscilloscope data capturing completed\n";

  vector<double> result(samplesBuffer, samplesBuffer + sampleBufferSize);
  return result;
}

/**
 * Measure frequency of a samples in provided buffer
 *
 * @param samples buffer
 * @return signal frequency
 */
double Oscilloscope::measureFrequency(const vector<double> &samples) const {
  vector<double> samplesBuffer(samples);

  double windowBuffer[samplesBuffer.size()];
  // DwfWindowFlatTop = 9
  FDwfSpectrumWindow(windowBuffer, samplesBuffer.size(), 9, 1, NULL);
  for (unsigned int i = 0; i < samplesBuffer.size(); i++) {
    samplesBuffer[i] *= windowBuffer[i];
  }

  int binsCount = samplesBuffer.size() / 2 + 1;
  double fBins[binsCount];
  FDwfSpectrumFFT(samplesBuffer.data(), samplesBuffer.size(), fBins, NULL, binsCount);

  int maxBinIndex = 0;
  double maxBinValue = 0;
  for (int i = dcOffsetBinIndex; i < binsCount; i++) {
    if (fBins[i] > maxBinValue) {
      maxBinValue = fBins[i];
      maxBinIndex = i;
    }
  }

  double hzRate;
  FDwfAnalogInFrequencyGet(deviceHandler.getDevice(), &hzRate);

  double maxFrequency = (hzRate / 2 * maxBinIndex) / (binsCount - 1);
  cout << "Frequency: " << maxFrequency << "\n";

  return maxFrequency;
}