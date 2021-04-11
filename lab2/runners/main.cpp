#include <string>
#include "../src/PNMReaderWriter.hpp"
#include "../src/PNMAutoBrightener.hpp"

int main(int argc, char *argv[]) {
    std::string rootFolder = "/home/ct/projects/multithreading-labs/lab2/images";
    std::string imageType = "pgm";
    std::string imageName = "baboon";
    std::string imageInputPath = rootFolder + "/" + imageType + "/" + imageName + "." + imageType;
    std::string imageOutputPath = rootFolder + "/" + imageType + "/" + imageName + "-new." + imageType;

    auto imageData = PNMReaderWriter::readFromFile(imageInputPath);
    auto autoBrightener = PNMAutoBrightener::withoutOMP();
    autoBrightener->correctImage(*imageData);
    PNMReaderWriter::writeToFile(imageOutputPath, *imageData);

    delete imageData;
    delete autoBrightener;

    return 0;
}