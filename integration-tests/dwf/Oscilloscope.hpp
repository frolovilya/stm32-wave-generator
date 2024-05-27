#ifndef SCOPE_HPP
#define SCOPE_HPP

#include "DwfDeviceHandler.hpp"
#include <vector>

// Wave generation output sampling frequency is 48KHz
// Capturing the signal at 100KHz
constexpr int defaultScopeFrequency = 100000;
constexpr int defaultScopeChannel = 1;
constexpr int defaultSampleBufferSize = 16384;
// start frequency analysis from this bin index to filter out DC offset
// frequencies
constexpr int dcOffsetBinIndex = 5;

class Oscilloscope {
public:
  Oscilloscope(const DwfDeviceHandler &deviceHandler, int frequency,
               int channel, int sampleBufferSize);
  std::vector<double> captureSamples() const;
  double measureFrequency(const std::vector<double> &samples) const;

private:
  const DwfDeviceHandler &deviceHandler;
  const int frequency;
  const int channelIndex;
  const int sampleBufferSize;

  void configure();
};

#endif
