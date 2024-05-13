#ifndef FACE_CORRESPONDENCES_H
#define FACE_CORRESPONDENCES_H

#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing.h>
#include <dlib/opencv/cv_image.h>
#include <dlib/image_io.h>

// Structure to store data about face correspondences including image dimensions, cropped images, and landmark lists.
struct FaceCorrespondenceData {
    std::vector<int> size;                            // Width and height of the cropped images
    cv::Mat cropped1;                                 // First cropped image
    cv::Mat cropped2;                                 // Second cropped image
    std::vector<cv::Point2f> list1;                   // Landmarks of the first image
    std::vector<cv::Point2f> list2;                   // Landmarks of the second image
    std::vector<cv::Point2f> corresp;                 // Averaged coordinates of landmarks
};

// Prints an error message if no face is detected
void print_error_message();

// Helper function to calculate cropping margins
void calculate_margin_help(const cv::Mat& img1, const cv::Mat& img2, std::vector<int>& results);

// Crops images to make them the same size
void crop_image(const cv::Mat& img1, const cv::Mat& img2, cv::Mat& cropped1, cv::Mat& cropped2);

// Generates face correspondence data
FaceCorrespondenceData generate_face_correspondences(const std::string& filename1, const std::string& filename2);

#endif // FACE_CORRESPONDENCES_H
