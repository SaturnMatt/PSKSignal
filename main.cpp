#include <iostream>
#include <vector>
#include <cmath>

#include "rnd.h"
#include "timer.h"
#include "signal.h"
#include "wavfile.h"

using namespace std;

int main() {

    const int bitCount = 1024;
    const int sampleRate = 44100;
    const float frequency = 1000;
    const float bitRate = 250;

    cout << "Data: " << bitCount << " bits" << endl;
    cout << "Speed: " << bitRate << " bits/s" << endl;
    cout << "Duration: " << bitCount / bitRate << " s" << endl;

    auto bits = UniformRandomInt<short>(0, 1).generate(bitCount);

    Timer timer;

    Signal signal(sampleRate);
    signal.modulateBPSK(bits, frequency, bitRate);
    WavFile().writeSignal(signal, "wavfile.wav");

    const float windowTime = 1.0 / bitRate;
    Signal demodulated = signal.dftWindow(frequency, windowTime);
    timer.stop();

    cout << "Sample Count: " << signal.samples.size() << endl;
    cout << "Window Sample count: " << windowTime * sampleRate << endl;
    cout << "Modulate/Demodulate took: " << timer << endl;
    
    WavFile().writeSignal(demodulated, "demod.wav");

    return 0;
}