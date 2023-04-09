#pragma once

#include <string>
#include <fstream>
#include <cstdint>

#include "signal.h"

class WavFile {
private:
    struct WavHeader {
        char riff[4]; // "RIFF"
        uint32_t fileSize;
        char wave[4]; // "WAVE"
        char fmt[4];  // "fmt "
        uint32_t fmtChunkSize;
        uint16_t audioFormat;
        uint16_t numChannels;
        uint32_t sampleRate;
        uint32_t byteRate;
        uint16_t blockAlign;
        uint16_t bitsPerSample;
        char data[4]; // "data"
        uint32_t dataSize;
    };

    double clampAmplitude(double x) {
        if (x < -maxAmplitude) return -maxAmplitude;
        if (x > maxAmplitude) return maxAmplitude;
        return x;
    }

    WavHeader header;
    const int16_t maxAmplitude = 32767;

public:
    WavFile() {
        memset(&header, 0, sizeof(WavHeader));
        memcpy(header.riff, "RIFF", 4);
        memcpy(header.wave, "WAVE", 4);
        memcpy(header.fmt, "fmt ", 4);
        header.fmtChunkSize = 16;
        header.audioFormat = 1;
        header.numChannels = 1;
        header.bitsPerSample = 16;
        memcpy(header.data, "data", 4);
    }

    void writeSignal(const Signal& signal, const std::string& filename) {
        header.sampleRate = static_cast<uint32_t>(signal.sampleRate);
        header.byteRate = header.sampleRate * header.numChannels * header.bitsPerSample / 8;
        header.blockAlign = header.numChannels * header.bitsPerSample / 8;
        header.dataSize = static_cast<uint32_t>(signal.samples.size() * header.blockAlign);
        header.fileSize = header.dataSize + sizeof(WavHeader) - 8;

        std::ofstream outFile(filename, std::ios::binary);
        outFile.write(reinterpret_cast<const char*>(&header), sizeof(WavHeader));

        for (auto sample : signal.samples) {
            int16_t intSample = static_cast<int16_t>(clampAmplitude(sample * maxAmplitude));
            outFile.write(reinterpret_cast<const char*>(&intSample), sizeof(int16_t));
        }

        outFile.close();
    }
};
