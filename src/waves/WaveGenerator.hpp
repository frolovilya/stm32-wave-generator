#ifndef WAVEGENERATOR_HPP
#define WAVEGENERATOR_HPP

#include <cmath>
#include <stdexcept>
#include <vector>

template <typename T> class WaveGenerator {
public:
  WaveGenerator(const int samplingRate) : samplingRate{samplingRate} {
    checkSamplingRate(samplingRate);
  }

  virtual ~WaveGenerator() {}

  virtual std::vector<T> generatePeriod(const int frequency, const T amplitude,
                                        const double phaseShift) const = 0;

  /**
   * Nyquist frequency defines max possible frequency
   * to be digitally captured with a given sampling rate
   *
   * @param samplingRate sampling rate (Hz) (must be >=1)
   * @return max possible frequency (Hz)
   */
  static int nyquistFrequency(const int samplingRate) {
    checkSamplingRate(samplingRate);

    return samplingRate / 2;
  }

  /**
   * Calculate how many samples will take a single-period wave with a given
   * frequency and sampling rate
   *
   * @param samplingRate sampling rate (Hz) (must be >=1)
   * @param frequency target wave frequency (must be >= 1 and <= Nyquist
   * frequency)
   * @return number of samples to represent one period
   */
  static int calculatePeriodSamplesCount(const int samplingRate,
                                         const int frequency) {
    checkSamplingRate(samplingRate);
    checkFrequency(frequency);
    checkFrequencyVsSamplingRate(frequency, samplingRate);
  
    return std::ceil(samplingRate / (double)frequency);
  }

protected:
  const int samplingRate;

  static void checkSamplingRate(const int samplingRate) {
    if (samplingRate < 1) {
      throw std::invalid_argument("samplingRate must be >= 1");
    }
  }
  static void checkFrequency(const int frequency) {
    if (frequency < 1) {
      throw std::invalid_argument("frequency must be >= 1");
    }
  }
  static void checkFrequencyVsSamplingRate(const int frequency, const int samplingRate) {
    if (frequency > nyquistFrequency(samplingRate)) {
      throw std::invalid_argument(
          "frequency must be < samplingRate / 2 (Nyquist frequency)");
    }
  }
  static void checkAmplitude(const T amplitude) {
    if (amplitude < 0) {
      throw std::invalid_argument("amplitude must be positive");
    }
  }
};

#endif