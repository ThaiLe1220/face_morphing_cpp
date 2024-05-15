#include "video_handler.h"

std::vector<std::pair<cv::Mat, int>> extractFramesWithDuration(const std::string& videoPath, int outputResolution, int outputFPS, int start, int duration) {
    // Create a VideoCapture object
    cv::VideoCapture cap(videoPath);

    // Check if the video opened successfully
    if (!cap.isOpened()) {
        std::cerr << "Error: Unable to open video file." << std::endl;
        return {};
    }

    // Get video FPS
    double videoFPS = cap.get(cv::CAP_PROP_FPS);

    // Calculate start frame index and end frame index based on start time and duration
    int startFrameIndex = start * videoFPS;
    int endFrameIndex = std::min(startFrameIndex + duration * videoFPS, cap.get(cv::CAP_PROP_FRAME_COUNT));

    // Calculate aspect ratio
    cv::Size frameSize(cap.get(cv::CAP_PROP_FRAME_WIDTH), cap.get(cv::CAP_PROP_FRAME_HEIGHT));
    double aspectRatio = static_cast<double>(frameSize.height) / frameSize.width;

    // Calculate output width and height based on output resolution
    int outputWidth = outputResolution;
    int outputHeight = outputResolution * aspectRatio;

    // Initialize frames list
    std::vector<std::pair<cv::Mat, int>> framesWithTimestamps;

    // Set frame index to start frame
    cap.set(cv::CAP_PROP_POS_FRAMES, startFrameIndex);

    // Loop through frames within specified duration
    while (cap.isOpened() && cap.get(cv::CAP_PROP_POS_FRAMES) < endFrameIndex) {
        // Get the current frame index
        double currentFrameIndex = cap.get(cv::CAP_PROP_POS_FRAMES);

        // Calculate the timestamp for the current frame
        int currentTimestamp = currentFrameIndex / videoFPS * 1000;

        // Read the next frame
        cv::Mat frame;
        cap.read(frame);

        if (frame.empty())
            break;

        // Resize the frame to the specified output resolution
        cv::resize(frame, frame, cv::Size(outputWidth, outputHeight));

        // Append the frame and its timestamp to the list
        framesWithTimestamps.emplace_back(frame, currentTimestamp);

        // Skip frames according to the output FPS
        for (int i = 0; i < videoFPS / outputFPS - 1; ++i)
            cap.grab();
    }

    // Release the VideoCapture object
    cap.release();

    return framesWithTimestamps;
}
