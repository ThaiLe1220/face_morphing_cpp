#include "video_processor.h"
#include <chrono>

void VideoProcessor::convertVideoToGrayscale(const std::string& inputFilePath, const std::string& outputFilePath) {
    cv::VideoCapture cap(inputFilePath);
    if (!cap.isOpened()) {
        std::cerr << "Error: Could not open the input video file." << std::endl;
        return;
    }

    int frame_width = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_WIDTH));
    int frame_height = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_HEIGHT));
    int original_fps = static_cast<int>(cap.get(cv::CAP_PROP_FPS));
    int target_fps = 30;

    if (original_fps > target_fps) {
        std::cout << "Original FPS is " << original_fps << ", reducing to " << target_fps << " FPS." << std::endl;
    }
    else {
        target_fps = original_fps;
    }

    cv::VideoWriter writer(outputFilePath, cv::VideoWriter::fourcc('m', 'p', '4', 'v'), target_fps, cv::Size(frame_width, frame_height), false);

    if (!writer.isOpened()) {
        std::cerr << "Error: Could not open the output video file." << std::endl;
        return;
    }

    cv::Mat frame, grayFrame;
    int frame_count = 0;
    int skip_frames = original_fps / target_fps;

    auto start_read = std::chrono::high_resolution_clock::now();
    auto total_read_time = std::chrono::duration<double>::zero();
    auto total_convert_time = std::chrono::duration<double>::zero();
    auto total_write_time = std::chrono::duration<double>::zero();

    while (true) {
        auto start_frame_read = std::chrono::high_resolution_clock::now();
        if (!readFrameFromVideo(cap, frame)) {
            break;
        }
        auto end_frame_read = std::chrono::high_resolution_clock::now();
        total_read_time += end_frame_read - start_frame_read;

        if (original_fps > target_fps && frame_count % skip_frames != 0) {
            frame_count++;
            continue;
        }

        auto start_convert = std::chrono::high_resolution_clock::now();
        convertFrameToGrayscale(frame, grayFrame);
        auto end_convert = std::chrono::high_resolution_clock::now();
        total_convert_time += end_convert - start_convert;

        auto start_write = std::chrono::high_resolution_clock::now();
        writer.write(grayFrame);
        auto end_write = std::chrono::high_resolution_clock::now();
        total_write_time += end_write - start_write;

        frame_count++;
    }

    cap.release();
    writer.release();

    auto end_read = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> total_time = end_read - start_read;

    std::cout << "Total video read time: " << total_read_time.count() << " seconds." << std::endl;
    std::cout << "Total frame to grayscale conversion time: " << total_convert_time.count() << " seconds." << std::endl;
    std::cout << "Total frame write time: " << total_write_time.count() << " seconds." << std::endl;
    std::cout << "Total processing time: " << total_time.count() << " seconds." << std::endl;
}

bool VideoProcessor::readFrameFromVideo(cv::VideoCapture& cap, cv::Mat& frame) {
    return cap.read(frame);
}

void VideoProcessor::convertFrameToGrayscale(const cv::Mat& frame, cv::Mat& grayFrame) {
    cv::cvtColor(frame, grayFrame, cv::COLOR_BGR2GRAY);
}
