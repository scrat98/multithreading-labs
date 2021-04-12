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

double measureAvgTimeMillis(const std::function<void()> &fun, int timesToRun = 10) {
    double elapsedTimeAvg = 0;
    for (int i = 0; i < timesToRun; i++) {
        auto elapsedTime = measureTimeMillis(fun);
        elapsedTimeAvg += elapsedTime;
    }
    return elapsedTimeAvg / timesToRun;
}

#endif /* utils.h */
