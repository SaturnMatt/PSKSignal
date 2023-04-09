#include <iostream>
#include <vector>
#include <cmath>

#include "rnd.h"
#include "timer.h"
#include "signal.h"
#include "wavfile.h"

using namespace std;

int main() {

    const int bitCount = 16;
    const int sampleRate = 44100;
    const float frequency = 10000;
    const float bitRate = 10;

    cout << "Data: " << bitCount << " bits" << endl;
    cout << "Speed: " << bitRate << " bits/s" << endl;
    cout << "Duration: " << bitCount / bitRate << " s" << endl;

    auto bits = UniformRandomInt<short>(0, 1).generate(bitCount);

    Signal signal(sampleRate);
    signal.modulateBPSK(bits, frequency, bitRate, 0.05);
    signal.addNoise(0.9);
    WavFile().writeSignal(signal, "wav_mod.wav");

    Timer timer;
    const float windowTime = 1.0 / bitRate;
    Signal demodulated = signal.dftWindow(frequency, windowTime);
    timer.stop();

    cout << "Sample Count: " << signal.samples.size() << endl;
    cout << "Window Sample Count: " << windowTime * sampleRate << endl;
    cout << "Demodulate Time: " << timer << endl;
    
    WavFile().writeSignal(demodulated, "wav_demod.wav");

    return 0;
}