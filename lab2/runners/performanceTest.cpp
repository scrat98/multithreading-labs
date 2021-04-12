#include <cstdio>
#include <map>
#include <random>
#include "../src/PNMReaderWriter.hpp"
#include "../src/PNMAutoBrightener.hpp"
#include "../src/utils.h"

ImageData *getImageData(int imageSize) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<unsigned char> dis(10, UINT8_MAX - 10);

    int dataSize = imageSize * imageSize;
    auto imageData = new unsigned char[dataSize];
    for (int i = 0; i < dataSize; i++) {
        imageData[i] = dis(gen);
    }

    return new ImageData("P5", imageSize, imageSize, 255, imageData, dataSize);
}

int main() {
    std::vector<int> numberOfThreadsToTest = {0, 1, 2, 4, 6, 8, 16};
    std::map<int, std::map<int, long long>> results;

    for (int imageSize = 100; imageSize < 5000; imageSize += 50) {
        auto imageData = getImageData(imageSize);
        std::printf("Image size: %d\n", imageSize);

        for (int numberOfThreads: numberOfThreadsToTest) {
            auto autoBrightener = PNMAutoBrightener::withOMP(numberOfThreads);
            auto elapsedTimeAvg = measureAvgTimeMillis([&] {
                auto pixelDataCopy = new unsigned char[imageData->dataSize];
                std::copy(imageData->pixelData, imageData->pixelData + imageData->dataSize, pixelDataCopy);
                auto imageDataCopy = ImageData(imageData->magicNumber,
                                               imageData->width, imageData->height, imageData->maxValue,
                                               pixelDataCopy, imageData->dataSize
                );
                autoBrightener->correctImage(imageDataCopy);
            });
            std::printf("Threads: %d, Time: %lf\n", numberOfThreads, elapsedTimeAvg);
            results[imageSize][numberOfThreads] = elapsedTimeAvg;
        }

        auto autoBrightener = PNMAutoBrightener::withoutOMP();
        auto elapsedTimeAvg = measureAvgTimeMillis([&] {
            auto pixelDataCopy = new unsigned char[imageData->dataSize];
            std::copy(imageData->pixelData, imageData->pixelData + imageData->dataSize, pixelDataCopy);
            auto imageDataCopy = ImageData(imageData->magicNumber,
                                           imageData->width, imageData->height, imageData->maxValue,
                                           pixelDataCopy, imageData->dataSize
            );
            autoBrightener->correctImage(imageDataCopy);
        });
        std::printf("Threads: -1, Time: %lf\n", elapsedTimeAvg);
        results[imageSize][-1] = elapsedTimeAvg;

        delete imageData;
    }

    auto resFile = fopen("result.csv", "w");
    std::fprintf(resFile, "Image size");
    for (auto const &res: results.begin()->second) {
        std::fprintf(resFile, ", thread-<%d>", res.first);
    }
    std::fprintf(resFile, "\n");

    for (auto const&[imageSize, result]: results) {
        std::fprintf(resFile, "%d", imageSize);
        for (auto const &res: result) {
            std::fprintf(resFile, ", %lld", res.second);
        }
        std::fprintf(resFile, "\n");
    }

    return 0;
}