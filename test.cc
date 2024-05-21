#include <iostream>
#include <chrono>
#include "video_processor.h"

int main() {
    std::string inputFilePath = "img/clip1.mp4";
    std::string outputFilePath = "img/clip1_g.mp4";

    auto start = std::chrono::high_resolution_clock::now();

    VideoProcessor::convertVideoToGrayscale(inputFilePath, outputFilePath);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    std::cout << "Total processing time (including setup): " << duration.count() << " seconds." << std::endl;

    return 0;
}
