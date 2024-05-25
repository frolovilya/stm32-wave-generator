#ifndef SINEWAVE_HPP
#define SINEWAVE_HPP

#include "WaveGenerator.hpp"
#include <numbers>

template <typename T> class SineWave : public WaveGenerator<T> {
public:
  using WaveGenerator<T>::WaveGenerator;

protected:
  inline T calculateSampleAtIndex(const int i, const int period,
                                  const T amplitude,
                                  const double phaseShift) const override {
    return (sin(2 * std::numbers::pi * i / period + phaseShift) +
            this->dcOffset) *
           amplitude / (1 + this->dcOffset);
  }
};

#endif