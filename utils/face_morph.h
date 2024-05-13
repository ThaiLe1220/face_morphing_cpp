#ifndef FACE_MORPH_H
#define FACE_MORPH_H

#include <vector>
#include <tuple>
#include <opencv2/opencv.hpp>

// Declaration for the function that applies affine transformation
cv::Mat apply_affine_transform(cv::Mat src, std::vector<cv::Point2f>& srcTri, std::vector<cv::Point2f>& dstTri, cv::Size size);

// Declaration for the function that morphs one triangle at a time
void morph_triangle(cv::Mat& img1, cv::Mat& img2, cv::Mat& img, std::vector<cv::Point2f>& t1, std::vector<cv::Point2f>& t2, std::vector<cv::Point2f>& t, float alpha);

// Declaration for the function that morphs two images based on a given alpha blending factor
cv::Mat morph_images(cv::Mat& img1, cv::Mat& img2, std::vector<cv::Point2f>& points1, std::vector<cv::Point2f>& points2, std::vector<std::tuple<int, int, int>>& tri_list, float alpha);

#endif // FACE_MORPH_H
