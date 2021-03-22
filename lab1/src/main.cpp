#include <iostream>
#include "DeterminantCalculator.hpp"
#include "utils.h"

int main() {
    Matrix matrix = {
            {1, 2, 8.8, 9.9},
            {3.0, 4.0, 3.3, 4.4},
            {2.0, 5.2, 2.2, 5.5},
            {6.3, 7.8, 6.3, 7.8}
    };
    auto detCalculator = DeterminantCalculator::withOMP(omp_sched_auto, 8);

    float det;
    auto elapsedTime = measureTimeMillis([&] { det = detCalculator->calculate(matrix); });

    std::cout << "Determinant: " << det << std::endl;
    std::cout << "Time (8 thread(s)): " << elapsedTime << "ms" << std::endl;

    delete detCalculator;

    return 0;
}