#ifndef MULTITHREADING_LABS_PNMREADERWRITER_HPP
#define MULTITHREADING_LABS_PNMREADERWRITER_HPP

#include <string>
#include <fstream>

struct ImageData {
    std::string magicNumber;
    int width, height, maxValue;
    unsigned char *pixelData;
    int dataSize;

    ImageData(std::string magicNumber,
              int width, int height, int maxValue,
              unsigned char *pixelData, int dataSize
    ) : magicNumber(magicNumber),
        width(width), height(height), maxValue(maxValue),
        pixelData(pixelData), dataSize(dataSize) {}
};

class PNMReaderWriter {
public:
    static ImageData *readFromFile(const std::string &imageFilePath) {
        std::ifstream imageFile;
        imageFile.open(imageFilePath, std::ifstream::in | std::ifstream::binary);
        if (!imageFile.is_open()) {
            throw std::runtime_error("Could not open the image file: " + imageFilePath);
        }

        std::string magicNumber;
        imageFile >> magicNumber;
        int channelsNumber;
        if (magicNumber == "P5") {
            channelsNumber = 1;
        } else if (magicNumber == "P6") {
            channelsNumber = 3;
        } else {
            throw std::runtime_error("Unsupported image type: " + magicNumber);
        }

        int width, height, maxValue;
        imageFile >> width >> height >> maxValue;
        imageFile.ignore(1);

        int dataSize = width * height * channelsNumber;
        auto data = new unsigned char[dataSize];
        imageFile.read(reinterpret_cast<char *>(data), dataSize);
        imageFile.close();

        return new ImageData(magicNumber, width, height, maxValue, data, dataSize);
    }

    static void writeToFile(const std::string &imageFilePath, const ImageData &imageData) {
        std::ofstream imageFile;
        imageFile.open(imageFilePath, std::ofstream::out | std::ofstream::binary);
        if (!imageFile.is_open()) {
            throw std::runtime_error("Could not open the image file: " + imageFilePath);
        }

        imageFile << imageData.magicNumber << '\n'
                  << imageData.width << ' ' << imageData.height << '\n'
                  << imageData.maxValue << '\n';
        imageFile.write(reinterpret_cast<char *>(imageData.pixelData), imageData.dataSize);
        imageFile.close();
    }
};


#endif /* PNMReaderWriter.hpp */
