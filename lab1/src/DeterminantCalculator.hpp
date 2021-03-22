#ifndef MULTITHREADING_LABS_DETERMINANTCALCULATOR_HPP
#define MULTITHREADING_LABS_DETERMINANTCALCULATOR_HPP

#include <omp.h>
#include <vector>

typedef std::vector<std::vector<float>> Matrix;

class DeterminantCalculator {
public:
    DeterminantCalculator(omp_sched_t scheduleType, int numOfThreads);

    ~DeterminantCalculator();

    float calculate(Matrix matrix);

private:
    omp_sched_t scheduleType;
    int numOfThreads;
};

#endif /* DeterminantCalculator.h */
