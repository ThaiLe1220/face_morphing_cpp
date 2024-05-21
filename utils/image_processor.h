#ifndef IMAGE_PROCESSOR_H
#define IMAGE_PROCESSOR_H

#include <opencv2/opencv.hpp>
#include <string>

class ImageProcessor
{
public:
    static void convertToGrayScale(const std::string& inputImagePath, const std::string& outputImagePath);
    static void captureFrame(const std::string& videoPath, const std::string& outputDir);
};

#endif // IMAGE_PROCESSOR_H
