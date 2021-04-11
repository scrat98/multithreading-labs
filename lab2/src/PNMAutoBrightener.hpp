#ifndef MULTITHREADING_LABS_PNMAUTOBRIGHTENER_HPP
#define MULTITHREADING_LABS_PNMAUTOBRIGHTENER_HPP

#include "PNMReaderWriter.hpp"

class PNMAutoBrightener {
public:
    static PNMAutoBrightener *withOMP(unsigned int numOfThreads = 0) {
        return new PNMAutoBrightener(numOfThreads);
    }

    static PNMAutoBrightener *withoutOMP() {
        return new PNMAutoBrightener();
    }

    void correctImage(const ImageData &imageData) const;

private:
    PNMAutoBrightener(unsigned int numOfThreads) : numOfThreads(numOfThreads),
                                                   useOmp(true) {};

    PNMAutoBrightener() : useOmp(false) {};

    bool useOmp;
    unsigned int numOfThreads;
};


#endif /* PNMAutoBrightener.hpp */
