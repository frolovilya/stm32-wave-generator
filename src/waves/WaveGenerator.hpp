#ifndef WAVEGENERATOR_HPP
#define WAVEGENERATOR_HPP

#include <cmath>
#include <stdexcept>
#include <type_traits>
#include <vector>

template <typename T> class WaveGenerator {
public:
  explicit WaveGenerator(const int samplingRate)
      : samplingRate{samplingRate}, dcOffset([] {
          if constexpr (std::is_unsigned<T>()) {
            return 1;
          } else {
            return 0;
          }
        }()) {
    checkSamplingRate(samplingRate);
  }

  virtual ~WaveGenerator() {}

  /**
   * Generate one wave period
   *
   * @param frequency target wave frequency (must be >= 1)
   * @param amplitude max wave amplitude (must be > 0)
   * @param phaseShift shift wave start in radians
   * @return sine wave one period samples
   */
  virtual std::vector<T> generatePeriod(const int frequency, const T amplitude,
                                        const double phaseShift = 0) const {
    checkFrequency(frequency);
    checkAmplitude(amplitude);

    const int period = calculatePeriodSamplesCount(samplingRate, frequency);
    std::vector<T> samples;
    samples.reserve(period);

    for (int i = 0; i < period; i++) {
      samples.push_back(
          calculateSampleAtIndex(i, period, amplitude, phaseShift));
    }

    return samples;
  }

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
  const int dcOffset;

  static int modulo(int x, int p) { return ((x % p) + p) % p; }

  virtual inline T calculateSampleAtIndex(const int i, const int period,
                                          const T amplitude,
                                          const double phaseShift) const = 0;

private:
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
  static void checkFrequencyVsSamplingRate(const int frequency,
                                           const int samplingRate) {
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