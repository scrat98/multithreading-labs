#include <cstdio>
#include <string>
#include "../src/PNMReaderWriter.hpp"
#include "../src/PNMAutoBrightener.hpp"
#include "../src/utils.h"

void run(const std::string &inFilePath, const std::string &outFilePath, int numOfThreads) {
    bool useOmp = numOfThreads != -1;
    PNMAutoBrightener *autoBrightener;
    if (useOmp) {
        if (numOfThreads < 0) {
            throw std::invalid_argument("Wrong number of threads. Must be -1(omp disabled), 0(default) or greater than.");
        }
        autoBrightener = PNMAutoBrightener::withOMP(numOfThreads);
    } else {
        autoBrightener = PNMAutoBrightener::withoutOMP();
    }

    auto imageData = PNMReaderWriter::readFromFile(inFilePath);
    auto elapsedTime = measureTimeMillis([&] { autoBrightener->correctImage(*imageData); });
    PNMReaderWriter::writeToFile(outFilePath, *imageData);

    std::printf("Time (%d thread(s)): %lfms\n", numOfThreads, elapsedTime);

    delete imageData;
    delete autoBrightener;
}

int main(int argc, char *argv[]) {
    try {
        if (argc != 4) {
            throw std::invalid_argument("Wrong number of arguments. Must be in format <in file> <out file> <number of threads>");
        }
        auto inFileName = std::string(argv[1]);
        auto outFileName = std::string(argv[2]);
        int numOfThreads = atoi(argv[3]);
        run(inFileName, outFileName, numOfThreads);
    } catch (const std::exception &e) {
        std::fprintf(stderr, "The program was aborted: %s", e.what());
        return 1;
    }
    return 0;
}