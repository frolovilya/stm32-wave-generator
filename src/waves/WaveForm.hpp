#ifndef WAVEFORM_HPP
#define WAVEFORM_HPP

#include <string>

typedef enum class WaveForm { SINE, SQUARE, SAW, TRIANGLE } WaveForm;

constexpr WaveForm defaultWaveForm = WaveForm::SINE;

std::string waveFormToString(const WaveForm waveForm);
WaveForm stringToWaveForm(const std::string str);

#endif