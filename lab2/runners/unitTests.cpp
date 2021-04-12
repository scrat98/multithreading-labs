#include <cstdio>
#include "../src/PNMReaderWriter.hpp"
#include "../src/PNMAutoBrightener.hpp"
#include "../src/utils.h"

int main() {
    std::string rootFolder = "/home/ct/projects/multithreading-labs/lab2/images";
    std::string imageType = "pgm";
    std::string imageName = "baboon";
    std::string imageInputPath = rootFolder + "/" + imageType + "/" + imageName + "." + imageType;
    std::string imageOutputPath = rootFolder + "/" + imageType + "/" + imageName + "-new." + imageType;
    int numOfThreads = 8;

    auto autoBrightener = PNMAutoBrightener::withOMP(numOfThreads);
    auto imageData = PNMReaderWriter::readFromFile(imageInputPath);
    auto elapsedTime = measureTimeMillis([&] { autoBrightener->correctImage(*imageData); });
    PNMReaderWriter::writeToFile(imageOutputPath, *imageData);

    std::printf("Time (%d thread(s)): %lfms\n", numOfThreads, elapsedTime);
}