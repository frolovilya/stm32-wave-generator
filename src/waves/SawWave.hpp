#ifndef SAWWAVE_HPP
#define SAWWAVE_HPP

#include "WaveGenerator.hpp"
#include <cmath>
#include <numbers>

template <typename T> class SawWave : public WaveGenerator<T> {
public:
  using WaveGenerator<T>::WaveGenerator;

protected:
  inline T calculateSampleAtIndex(const int i, const int period,
                                  const T amplitude,
                                  const double phaseShift) const override {
    (void)phaseShift;
    return amplitude / period * std::abs(this->modulo(i - period / 2, period));
  }
};

#endif