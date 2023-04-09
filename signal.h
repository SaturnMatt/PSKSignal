#pragma once
#include <vector>
#include <cmath>
#include <complex>

#include "rnd.h"

constexpr float PI = 3.14159265358979323846;
using f32 = float;
using u8 = unsigned char;

struct Signal {
    int sampleRate;
    std::vector<float> samples;

    Signal(float sampleRate) : sampleRate(sampleRate) {}

    void appendSilence(float duration) {
        int numSamples = duration * sampleRate;
        for (int i = 0; i < numSamples; i++) {
            samples.push_back(0.0);
        }
    }

    void appendSine(float frequency, float phase, float duration, float amplitude = 1) {
        int numSamples = duration * sampleRate;
        float timeOffset = static_cast<float>(samples.size()) / sampleRate;
        for (int i = 0; i < numSamples; i++) {
            float t = static_cast<float>(i) / sampleRate + timeOffset;
            float x = t * frequency * 2.0 * PI + phase;
            float y = amplitude * sin(x);
            samples.push_back(y);
        }
    }

    void filterBandPass(float frequency) {
    }

    void addNoise(float amplitude) {
        UniformRandom<float> random(-1, 1);
        for (auto& sample : samples) {
            sample += amplitude * random.generate();
        }
    }

    void modulateBPSK(std::vector<short> bits, float frequency, float bitRate, float amplitude = 1, float padding = 10) {
        const float duration = 1.0 / bitRate;
        appendSilence(duration * padding);
        for (auto bit : bits) {
            float phase = bit * PI;
            appendSine(frequency, phase, duration, amplitude);
        }
        appendSilence(duration * padding);
        filterBandPass(frequency);
    }

    Signal dftWindow(float frequency, float windowTime) const {
        auto dft = dftSignal(frequency);

        int windowSize = windowTime * sampleRate;
        int halfWindowSize = windowSize / 2;

        Signal signal(sampleRate);
        std::complex<float> sum = 0;
        for (int i = -halfWindowSize; i < 0; i++) {
            sum += dft[i + halfWindowSize];
        }
        for (int i = 0; i < samples.size(); i++) {
            if (i + halfWindowSize < samples.size()) {
                sum += dft[i + halfWindowSize];
            }
            if (i - halfWindowSize >= 0) {
                sum -= dft[i - halfWindowSize];
            }
            auto dft = sum / static_cast<float>(windowSize);
            float amplitude = std::abs(dft);
            float phase = std::arg(dft);
            signal.samples.push_back(0.5 * amplitude * phase);
        }
        return signal;
    }

private:

    std::complex<float> dftIndex(float frequency, size_t index) const {
        float t = static_cast<float>(index) / sampleRate;
        float x = t * frequency * 2.0 * PI;
        return samples[index] * std::exp(std::complex<float>(0, -x));
    }

    std::vector<std::complex<float>> dftSignal(float frequency) const {
        std::vector<std::complex<float>> dft;
        for (int i = 0; i < samples.size(); i++) {
            dft.push_back(dftIndex(frequency, i));
        }
        return dft;
    }



};

