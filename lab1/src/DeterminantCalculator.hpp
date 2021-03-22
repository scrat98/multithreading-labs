#ifndef MULTITHREADING_LABS_DETERMINANTCALCULATOR_HPP
#define MULTITHREADING_LABS_DETERMINANTCALCULATOR_HPP

#include <omp.h>
#include <vector>

typedef std::vector<std::vector<float>> Matrix;

class DeterminantCalculator {
public:
    static DeterminantCalculator *withOMP(unsigned int numOfThreads = 0, omp_sched_t scheduleType = omp_sched_auto) {
        return new DeterminantCalculator(numOfThreads, scheduleType);
    }

    static DeterminantCalculator *withoutOMP() {
        return new DeterminantCalculator();
    }

    ~DeterminantCalculator() = default;

    float calculate(Matrix matrix) const;

private:
    inline static const float FLOAT_PRECISION = 1e-7;

    DeterminantCalculator(unsigned int numOfThreads, omp_sched_t scheduleType) : numOfThreads(numOfThreads),
                                                                                 scheduleType(scheduleType),
                                                                                 useOmp(true) {};

    DeterminantCalculator() : useOmp(false) {};

    bool useOmp;
    unsigned int numOfThreads;
    omp_sched_t scheduleType;
};

#endif /* DeterminantCalculator.h */
