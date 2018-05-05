#pragma once

#include <functional>
#include <ctime>
#include <chrono>

using Clock = std::chrono::high_resolution_clock;

inline long measureClocks(const std::function<void()> &function) {
    auto start = std::clock();
    function();
    return std::clock() - start;
}

inline long long int measureTimeNanos(const std::function<void()> &function) {
    auto start = Clock::now();
    function();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(Clock::now() - start).count();
}

inline long long int measureTimeMillis(const std::function<void()> &function) {
    auto start = Clock::now();
    function();
    return std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - start).count();
}