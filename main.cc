#include "face_correspondences.h"
#include "delaunay_triangulation.h"
#include "face_morph.h"
#include "video_handler.h"

#include <iostream>
#include <vector>
#include <tuple>
#include <opencv2/opencv.hpp>
#include <chrono>

int main() {

    // Path to input images
    std::string pathToFirstImage = "images/tam2.jpg";
    std::string pathToSecondImage = "images/taylor4.jpg";

    // Detect the target face
    dlib::frontal_face_detector detector = dlib::get_frontal_face_detector(); // Load Dlib's face detector
    dlib::shape_predictor predictor;
    dlib::deserialize("shape_predictor_68_face_landmarks.dat") >> predictor; // Load Dlib's shape predictor

    cv::Mat cvImg2 = cv::imread(pathToSecondImage);
    dlib::cv_image<dlib::bgr_pixel> dlib_img(cvImg2);
    std::vector<dlib::rectangle> dets = detector(dlib_img);

    if (dets.empty()) {
        std::cout << "Cannot find target face! (img2)" << std::endl;
        exit(1);
    }
    std::vector<cv::Point2f> lmLists2;
    for (auto& rect : dets) {
        dlib::full_object_detection shape = predictor(dlib_img, rect);
        for (int i = 0; i < 68; i++) {
            cv::Point2f pt(shape.part(i).x(), shape.part(i).y());
            lmLists2.push_back(pt);
        }
    }

    std::string videoPath = "Clips/clip2.mp4";
    std::vector<std::pair<cv::Mat, int>> frames_with_timestamp = extractFramesWithDuration(videoPath, 720);

    std::vector<std::tuple<int, int, int>> tri_lists = {
        {18, 37, 36}, {37, 18, 19}, {33, 50, 32}, {50, 33, 51}, {1, 0, 36}, {48, 49, 60}, {49, 48, 3}, {2, 1, 41}, {52, 33, 34}, {33, 52, 51}, {3, 2, 31}, {3, 31, 49}, {4, 3, 48}, {54, 35, 13}, {35, 54, 53}, {5, 4, 48}, {30, 35, 34}, {35, 30, 29}, {6, 5, 59}, {29, 31, 40}, {31, 29, 30}, {7, 6, 58}, {46, 14, 35}, {14, 46, 45}, {8, 7, 57}, {9, 8, 56}, {10, 9, 55}, {21, 27, 39}, {27, 21, 22}, {11, 10, 54}, {12, 11, 54}, {38, 19, 20}, {19, 38, 37}, {13, 12, 54}, {36, 0, 17}, {15, 13, 14}, {14, 13, 35}, {18, 36, 17}, {15, 14, 45}, {16, 15, 45}, {19, 23, 20}, {23, 19, 24}, {39, 27, 28}, {42, 35, 29}, {35, 42, 47}, {22, 21, 23}, {20, 23, 21}, {22, 43, 42}, {43, 22, 23}, {20, 21, 38}, {27, 42, 28}, {42, 27, 22}, {43, 23, 24}, {44, 25, 45}, {25, 44, 24}, {16, 45, 26}, {25, 26, 45}, {2, 41, 31}, {40, 31, 41}, {35, 53, 52}, {28, 29, 39}, {29, 28, 42}, {66, 58, 67}, {58, 66, 57}, {63, 51, 52}, {51, 63, 62}, {57, 7, 58}, {31, 30, 32}, {56, 8, 57}, {31, 32, 50}, {32, 30, 33}, {55, 9, 56}, {33, 30, 34}, {34, 35, 52}, {1, 36, 41}, {29, 40, 39}, {38, 21, 39}, {43, 24, 44}, {46, 35, 47}, {5, 48, 59}, {49, 31, 50}, {50, 61, 49}, {61, 50, 51}, {63, 52, 53}, {10, 55, 54}, {53, 54, 64}, {54, 55, 64}, {55, 56, 65}, {56, 57, 66}, {60, 67, 59}, {58, 6, 59}, {61, 60, 49}, {58, 59, 67}, {59, 48, 60}, {61, 51, 62}, {56, 66, 65}, {65, 64, 55}, {63, 64, 53}
    };

    // Initialize video writer
    cv::VideoWriter outputVideo;
    int frame_width = frames_with_timestamp[0].first.cols;
    int frame_height = frames_with_timestamp[0].first.rows;
    double fps = 30.0; // Assuming 30 fps, adjust as needed

    outputVideo.open("output_video.mp4", cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), fps, cv::Size(frame_width, frame_height), true);

    if (!outputVideo.isOpened()) {
        std::cerr << "Could not open the output video file for write\n";
        return -1;
    }

    for (int i = 0; i < frames_with_timestamp.size(); i++) {
        try {
            // Start the clock
            auto start = std::chrono::high_resolution_clock::now();
            
            std::cout << "Frame: " << i << std::endl;
            cv::Mat image = frames_with_timestamp[i].first;
            // Generate face correspondences between two images
            auto data = generate_face_correspondences(image, lmLists2);

            // Perform Delaunay triangulation on the averaged points
            // std::vector<std::tuple<int, int, int>> tri_lists = makeDelaunay(data.size[1], data.size[0], data.corresp);
            // std::cout << "Print here " << std::endl;

            // Morph the images using a blend factor of 0.5
            cv::Mat result = morph_images(data.img1, cvImg2, data.list1, data.list2, tri_lists, 0.5);

            // Write the morphed frame to the video file
            outputVideo.write(result);

            // Optional: Display the morphed result
            // cv::imshow("result", result);
            // cv::waitKey(1);

            auto finish = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> elapsed = finish - start;
            std::cout << "Elapsed time for frame " << i << " : " << elapsed.count() << " seconds." << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Error occurred: " << e.what() << std::endl;
        }
    }

    // Release the video writer
    outputVideo.release();

    return 0;
}
