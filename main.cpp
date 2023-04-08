#include <iostream>
#include <vector>
#include <cmath>

#include "rnd.h"
#include "signal.h"
#include "wavfile.h"

using namespace std;

int main() {

    const int bitCount = 8 * 64;
    const int sampleRate = 88200;
    const float frequency = 1000;
    const float bitRate = 500;

    cout << "Data: " << bitCount << " bits" << endl;
    cout << "Speed: " << bitRate << " bits/s" << endl;
    cout << "Duration: " << bitCount / bitRate << " s" << endl;

    auto bits = UniformRandomInt<int>(0, 1).generate(bitCount);

    const float duration = 1.0 / bitRate;
    Signal signal(sampleRate);
    signal.addSilence(duration * 10);
    for (auto bit : bits) {
        float phase = bit * PI;
        signal.addSine(frequency, phase, duration);
    }
    signal.addSilence(duration * 10);
    signal.filterBandPass(frequency);
    
    WavFile().writeSignal(signal, "wavfile.wav");

    return 0;
}