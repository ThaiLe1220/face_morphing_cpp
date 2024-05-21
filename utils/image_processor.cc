#include "image_processor.h"
#include <iostream>
#include <sstream>
#include <iomanip>

void ImageProcessor::convertToGrayScale(const std::string& inputImagePath, const std::string& outputImagePath)
{
    cv::Mat inputImage = cv::imread(inputImagePath, cv::IMREAD_COLOR);
    if (inputImage.empty())
    {
        throw std::runtime_error("Could not open or find the image: " + inputImagePath);
    }

    cv::Mat grayImage;
    cv::cvtColor(inputImage, grayImage, cv::COLOR_BGR2GRAY);

    if (!cv::imwrite(outputImagePath, grayImage))
    {
        throw std::runtime_error("Could not save the grayscale image: " + outputImagePath);
    }
}

void ImageProcessor::captureFrame(const std::string& videoPath, const std::string& outputDir)
{
    cv::VideoCapture cap(videoPath);
    if (!cap.isOpened())
    {
        throw std::runtime_error("Could not open video file: " + videoPath);
    }

    double fps = cap.get(cv::CAP_PROP_FPS);
    if (fps <= 0) fps = 30.0; // Default FPS to 30 if unable to retrieve
    int frameInterval = static_cast<int>(fps / 30.0);

    int frameCount = 0;
    cv::Mat frame;
    int frameIndex = 0;
    while (cap.read(frame))
    {
        if (frameIndex % frameInterval == 0)
        {
            std::ostringstream ss;
            ss << outputDir << "/frame_" << std::setw(5) << std::setfill('0') << frameCount << ".png";
            if (!cv::imwrite(ss.str(), frame))
            {
                throw std::runtime_error("Could not save frame to: " + ss.str());
            }
            std::cout << "Saved " << ss.str() << std::endl;
            frameCount++;
        }
        frameIndex++;
    }
}
