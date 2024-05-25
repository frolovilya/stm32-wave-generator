#ifndef TRIANGLEWAVE_HPP
#define TRIANGLEWAVE_HPP

#include "WaveGenerator.hpp"
#include <cmath>
#include <numbers>

template <typename T> class TriangleWave : public WaveGenerator<T> {
public:
  using WaveGenerator<T>::WaveGenerator;

protected:
  inline T calculateSampleAtIndex(const int i, const int period,
                                  const T amplitude,
                                  const double phaseShift) const override {
    (void)phaseShift;
    return 2 * amplitude / period *
           std::abs(this->modulo(i - period / 4, period) - period / 2);
  }
};

#endif