#ifndef VIDEO_HANDLER_H
#define VIDEO_HANDLER_H

#include <opencv2/opencv.hpp>
#include <vector>

std::vector<std::pair<cv::Mat, int>> extractFramesWithDuration(const std::string& videoPath, int outputResolution = 720, int outputFPS = 24, int start = 0, int duration = 60);

#endif // VIDEO_HANDLER_H
