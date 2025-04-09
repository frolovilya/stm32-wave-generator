#ifndef WAVEFACTORY_HPP
#define WAVEFACTORY_HPP

#include "WaveForm.hpp"
#include "Welle.hpp"
#include <string>
#include <vector>

// Max signal amplitude (value) for 12-bit DAC
constexpr double peakToPeak = 0xfff;

template <typename T>
std::vector<T> generateWavePeriod(const WaveForm waveForm,
                                  const int samplingRate, 
                                  const int frequency,
                                  const double peakToPeak,
                                  const double phaseShift = 0) {
  switch (waveForm) {
  case WaveForm::SQUARE:
    return welle::SquareWave<T>(samplingRate)
        .generatePeriod(frequency, peakToPeak, phaseShift);
  case WaveForm::SAW:
    return welle::SawWave<T>(samplingRate)
        .generatePeriod(frequency, peakToPeak, phaseShift);
  case WaveForm::TRIANGLE:
    return welle::TriangleWave<T>(samplingRate)
        .generatePeriod(frequency, peakToPeak, phaseShift);
  case WaveForm::SINE:
  default:
    return welle::SineWave<T>(samplingRate)
        .generatePeriod(frequency, peakToPeak, phaseShift);
  }
}

#endif