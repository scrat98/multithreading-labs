#ifndef MULTITHREADING_LABS_UTILS_H
#define MULTITHREADING_LABS_UTILS_H

#include <chrono>
#include <functional>

long measureTimeMillis(const std::function<void()> &fun) {
    auto start = std::chrono::high_resolution_clock::now();
    fun();
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}

#endif /* utils.h */
