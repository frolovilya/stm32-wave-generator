#ifndef WAVEFACTORY_HPP
#define WAVEFACTORY_HPP

#include "WaveForm.hpp"
#include "Welle.hpp"
#include <string>
#include <vector>

template <typename T>
std::vector<T> generateWavePeriod(const WaveForm waveForm,
                                  const int samplingRate, const int frequency,
                                  const double amplitude,
                                  const double phaseShift = 0) {
  switch (waveForm) {
  case WaveForm::SQUARE:
    return welle::SquareWave<T>(samplingRate)
        .generatePeriod(frequency, amplitude, phaseShift);
  case WaveForm::SAW:
    return welle::SawWave<T>(samplingRate)
        .generatePeriod(frequency, amplitude, phaseShift);
  case WaveForm::TRIANGLE:
    return welle::TriangleWave<T>(samplingRate)
        .generatePeriod(frequency, amplitude, phaseShift);
  case WaveForm::SINE:
  default:
    return welle::SineWave<T>(samplingRate)
        .generatePeriod(frequency, amplitude, phaseShift);
  }
}

#endif