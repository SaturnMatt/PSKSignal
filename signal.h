#pragma once
#include <vector>

constexpr float PI = 3.14159265358979323846;

struct Signal {
    int sampleRate;
    std::vector<float> samples;

    Signal(float sampleRate) : sampleRate(sampleRate) {}

    void addSilence(float duration) {
        int numSamples = duration * sampleRate;
        for (int i = 0; i < numSamples; i++) {
            samples.push_back(0.0);
        }
    }

    void addSine(float frequency, float phase, float duration) {
        int numSamples = duration * sampleRate;
        float timeOffset = static_cast<float>(samples.size()) / sampleRate;
        
        for (int i = 0; i < numSamples; i++) {
            float t = static_cast<float>(i) / sampleRate + timeOffset;
            float x = t * frequency * 2.0 * PI + phase;
            float y = 0.5f * sin(x);
            samples.push_back(y);
        }
    }

    void filterBandPass(float frequency) {
    }

};

