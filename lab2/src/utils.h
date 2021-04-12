#ifndef MULTITHREADING_LABS_UTILS_H
#define MULTITHREADING_LABS_UTILS_H

#include <chrono>
#include <functional>

double measureTimeMillis(const std::function<void()> &fun) {
    auto start = std::chrono::high_resolution_clock::now();
    fun();
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<double, std::milli>(end - start).count();
}

#endif /* utils.h */
