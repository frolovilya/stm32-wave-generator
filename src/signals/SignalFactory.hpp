#ifndef WAVEFACTORY_HPP
#define WAVEFACTORY_HPP

#include "Frequency.hpp"
#include "Level.hpp"
#include "WaveForm.hpp"
#include "Welle.hpp"
#include <string>
#include <vector>

template <typename T>
std::vector<T> generateSignalPeriod(const WaveForm waveForm,
                                    const int frequency,
                                    const double signalLevelVolts) {

  const int samplingRate = getSamplingRate();
  const double peakToPeak = getPeakToPeak(signalLevelVolts);
  const double phaseShift = 0;

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