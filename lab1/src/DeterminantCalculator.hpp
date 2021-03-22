#ifndef MULTITHREADING_LABS_DETERMINANTCALCULATOR_HPP
#define MULTITHREADING_LABS_DETERMINANTCALCULATOR_HPP

#include <omp.h>
#include <vector>

typedef std::vector<std::vector<float>> Matrix;

class DeterminantCalculator {
public:
    static DeterminantCalculator *withOMP(omp_sched_t scheduleType = omp_sched_auto, int numOfThreads = 0) {
        return new DeterminantCalculator(scheduleType, numOfThreads);
    }

    static DeterminantCalculator *withoutOMP() {
        return new DeterminantCalculator();
    }

    ~DeterminantCalculator() = default;

    float calculate(Matrix matrix) const;

private:
    inline static const float FLOAT_PRECISION = 1e-7;

    DeterminantCalculator(omp_sched_t scheduleType, int numOfThreads) : scheduleType(scheduleType),
                                                                        numOfThreads(numOfThreads),
                                                                        useOmp(true) {};

    DeterminantCalculator() : useOmp(false) {};

    bool useOmp;
    omp_sched_t scheduleType;
    int numOfThreads;
};

#endif /* DeterminantCalculator.h */
