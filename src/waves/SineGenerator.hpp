#ifndef SINEGENERATOR_HPP
#define SINEGENERATOR_HPP

#include "WaveGenerator.hpp"
#include <numbers>

template <typename T> class SineGenerator : public WaveGenerator<T> {
public:
  using WaveGenerator<T>::WaveGenerator;

  /**
   * Generate one period of a sine wave
   *
   * @param frequency target wave frequency (must be >= 1)
   * @param amplitude max wave amplitude (must be > 0)
   * @param phaseShift shift wave start
   * @return sine wave one period samples
   */
  std::vector<T> generatePeriod(const int frequency, const T amplitude,
                                const double phaseShift) const override {
    this->checkFrequency(frequency);
    this->checkAmplitude(amplitude);

    const int period =
        this->calculatePeriodSamplesCount(this->samplingRate, frequency);
    std::vector<T> samples;
    samples.reserve(period);
    for (int i = 0; i < period; i++) {
      samples.push_back(sin(2 * std::numbers::pi * i / period + phaseShift) *
                        amplitude);
    }

    return samples;
  }
};

#endif