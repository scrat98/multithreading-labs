#include "PNMAutoBrightener.hpp"

#include <algorithm>

void PNMAutoBrightener::correctImage(const ImageData &imageData) const {
    int histogram[256] = {0};
    #pragma omp parallel for if(useOmp) schedule(static) default(none) shared(histogram, imageData) num_threads(numOfThreads)
    for (int i = 0; i < imageData.dataSize; i++) {
        auto pixel = imageData.pixelData[i];
        #pragma omp atomic
        histogram[pixel]++;
    }

    int noisePixelsCount = imageData.dataSize / 256;
    int minNoisePixels = 0, maxNoisePixels = 0;
    unsigned char min = 0, max = 255;
    while (true) {
        minNoisePixels += histogram[min];
        if (minNoisePixels > noisePixelsCount) break;
        min++;
    }
    while (true) {
        maxNoisePixels += histogram[max];
        if (maxNoisePixels > noisePixelsCount) break;
        max--;
    }

    if (max == min) {
        return;
    }

    #pragma omp parallel for if(useOmp) schedule(static) default(none) shared(min, max, imageData) num_threads(numOfThreads)
    for (int i = 0; i < imageData.dataSize; i++) {
        auto transformedPixel = (imageData.pixelData[i] - min) * 255 / (max - min);
        imageData.pixelData[i] = std::clamp(transformedPixel, 0, 255);
    }
}