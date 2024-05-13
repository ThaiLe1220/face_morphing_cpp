#include <opencv2/opencv.hpp>
#include <string>
#include <iostream>

int main() {
    // Path to the video file
    std::string videoPath = "img/clip2.mp4";

    // Create a VideoCapture object to read from video file
    cv::VideoCapture cap(videoPath);
    if (!cap.isOpened()) {
        std::cerr << "Error: Could not open video file." << std::endl;
        return -1;
    }

    // Frame counter
    int frameNum = 0;

    // Read frames from the video
    cv::Mat frame;
    while (true) {
        cap >> frame; // Capture a frame

        // Check if the frame is empty
        if (frame.empty()) {
            break;
        }

        // Construct filename for each frame
        std::string filename = "frames/frame_" + std::to_string(frameNum++) + ".jpg";

        // Save frame to file
        cv::imwrite(filename, frame);
        std::cout << "Saved " << filename << std::endl;
    }

    // Release the video capture object
    cap.release();

    std::cout << "Finished extracting frames." << std::endl;
    return 0;
}
