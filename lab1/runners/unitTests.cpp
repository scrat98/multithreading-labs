#include <cstdio>
#include "../src/DeterminantCalculator.hpp"

int main() {
    Matrix matrix = {
            {1,   2,   8.8, 9.9},
            {3.0, 4.0, 3.3, 4.4},
            {2.0, 5.2, 2.2, 5.5},
            {6.3, 7.8, 6.3, 7.8}

    };
    auto calculator = DeterminantCalculator::withOMP();
    float det = calculator->calculate(matrix);
    std::printf("Det: %f", det);
}