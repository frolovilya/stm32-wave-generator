#ifndef SQUAREWAVE_HPP
#define SQUAREWAVE_HPP

#include "WaveGenerator.hpp"
#include <numbers>

template <typename T> class SquareWave : public WaveGenerator<T> {
public:
  using WaveGenerator<T>::WaveGenerator;

protected:
  inline T calculateSampleAtIndex(const int i, const int period,
                                  const T amplitude,
                                  const double phaseShift) const override {
    (void)phaseShift;
    return i < period / 2 ? amplitude : 0;
  }
};

#endif