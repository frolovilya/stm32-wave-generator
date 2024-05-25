#ifndef WAVEFACTORY_HPP
#define WAVEFACTORY_HPP

#include "SawWave.hpp"
#include "SineWave.hpp"
#include "SquareWave.hpp"
#include "TriangleWave.hpp"
#include "WaveForm.hpp"
#include <string>
#include <vector>

template <typename T>
std::vector<T> generateWavePeriod(const WaveForm waveForm,
                                  const int samplingRate, const int frequency,
                                  const T amplitude,
                                  const double phaseShift = 0) {
  switch (waveForm) {
  case WaveForm::SQUARE:
    return SquareWave<T>(samplingRate)
        .generatePeriod(frequency, amplitude, phaseShift);
  case WaveForm::SAW:
    return SawWave<T>(samplingRate)
        .generatePeriod(frequency, amplitude, phaseShift);
  case WaveForm::TRIANGLE:
    return TriangleWave<T>(samplingRate)
        .generatePeriod(frequency, amplitude, phaseShift);
  case WaveForm::SINE:
  default:
    return SineWave<T>(samplingRate)
        .generatePeriod(frequency, amplitude, phaseShift);
  }
}

#endif