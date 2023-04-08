#pragma once

#include <chrono>
#include <string>

class Timer {
public:
    Timer() : start_time(std::chrono::high_resolution_clock::now()), stopped(false) {}
    //void reset() { start_time = std::chrono::high_resolution_clock::now(); stopped = false; }
    void stop() { end_time = std::chrono::high_resolution_clock::now(); stopped = true; }
    double elapsed() const {
        auto end_t = stopped ? end_time : std::chrono::high_resolution_clock::now();
        auto elapsed_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end_t - start_time);
        return elapsed_time.count() * 1e-9;
    }
    std::string toString() const {
        double value = elapsed();
        std::string unit = "s";
        if (value < 1e-6) { value *= 1e9; unit = "ns"; }
        else if (value < 1e-3) { value *= 1e6; unit = "us"; }
        else if (value < 1) { value *= 1e3; unit = "ms"; }

        int whole = static_cast<int>(value);
        int frac = static_cast<int>((value - whole) * 1000);
        return std::to_string(whole) + "." + std::to_string(frac) + unit;
    }
    friend std::ostream& operator<<(std::ostream& os, const Timer& timer) { return os << timer.toString(); }
    void hang(double seconds) const {
        auto end = start_time + std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::duration<double>(seconds));
        while (std::chrono::high_resolution_clock::now() < end);
    }
private:
    std::chrono::high_resolution_clock::time_point start_time;
    std::chrono::high_resolution_clock::time_point end_time;
    bool stopped;
};
