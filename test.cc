#include <iostream>
#include "image_processor.h"

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <input_image_path> <output_image_path>" << std::endl;
        return 1;
    }

    std::string inputImagePath = argv[1];
    std::string outputImagePath = argv[2];

    try {
        ImageProcessor::convertToGrayScale(inputImagePath, outputImagePath);
        std::cout << "Image converted to grayscale and saved to " << outputImagePath << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
