#pragma once

#include <random>
#include <vector>

template <typename T = float>
class NormalRandom {
public:
    NormalRandom(T mean, T stddev)
        : random_device_(), engine_(random_device_()), distribution_(mean, stddev) {}
    NormalRandom() : NormalRandom(0, 1) {}

    T operator()() { return generate(); }
    T generate() { return distribution_(engine_); }
    std::vector<T> generate(size_t n) {
        std::vector<T> result(n);
        for (size_t i = 0; i < n; i++) {
            result[i] = distribution_(engine_);
        }
        return result;
    }

private:
    std::random_device random_device_;
    std::mt19937 engine_;
    std::normal_distribution<T> distribution_;
};

template <typename T = float>
class UniformRandom {
public:
    UniformRandom(T min, T max)
        : random_device_(), engine_(random_device_()), distribution_(min, max) {}
    UniformRandom() : UniformRandom(0, 1) {}

    T operator()() { return generate(); }
    T generate() { return distribution_(engine_); }
    std::vector<T> generate(size_t n) {
        std::vector<T> result(n);
        for (size_t i = 0; i < n; i++) {
            result[i] = distribution_(engine_);
        }
        return result;
    }

private:
    std::random_device random_device_;
    std::mt19937 engine_;
    std::uniform_real_distribution<T> distribution_;
};

template <typename T = int>
class UniformRandomInt {
public:
    UniformRandomInt(T min, T max)
        : random_device_(), engine_(random_device_()), distribution_(min, max) {}
    UniformRandomInt(T max) : UniformRandomInt(0, max) {}
    UniformRandomInt()
        : UniformRandomInt(
            std::numeric_limits<T>::min(), std::numeric_limits<T>::max()) {}

    template <typename U>
    U pick(const std::vector<U>& vec) {
        size_t idx = static_cast<size_t>(distribution_(engine_));
        idx = idx % vec.size();
        return vec[idx];
    }

    T operator()() { return generate(); }
    T generate() { return distribution_(engine_); }
    std::vector<T> generate(size_t n) {
        std::vector<T> result(n);
        for (size_t i = 0; i < n; i++) {
            result[i] = distribution_(engine_);
        }
        return result;
    }

private:
    std::random_device random_device_;
    std::mt19937 engine_;
    std::uniform_int_distribution<T> distribution_;
};
