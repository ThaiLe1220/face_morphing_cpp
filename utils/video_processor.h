#ifndef VIDEO_PROCESSOR_H
#define VIDEO_PROCESSOR_H

#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>

class VideoProcessor {
public:
    static void convertVideoToGrayscale(const std::string& inputFilePath, const std::string& outputFilePath);
private:
    static bool readFrameFromVideo(cv::VideoCapture& cap, cv::Mat& frame);
    static void convertFrameToGrayscale(const cv::Mat& frame, cv::Mat& grayFrame);
};

#endif // VIDEO_PROCESSOR_H
