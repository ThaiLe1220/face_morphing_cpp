#include "face_morph.h"

// Function to apply affine transformation to a source image
cv::Mat apply_affine_transform(cv::Mat src, std::vector<cv::Point2f>& srcTri, std::vector<cv::Point2f>& dstTri, cv::Size size) {
    // Calculate the affine transform matrix
    cv::Mat warpMat = cv::getAffineTransform(srcTri, dstTri);
    cv::Mat dst;
    // Apply the affine transform
    cv::warpAffine(src, dst, warpMat, size, cv::INTER_LINEAR, cv::BORDER_REFLECT_101);
    return dst;
}

// Function to morph a triangle from two source images to a destination image
void morph_triangle(cv::Mat& img1, cv::Mat& img2, cv::Mat& img, std::vector<cv::Point2f>& t1, std::vector<cv::Point2f>& t2, std::vector<cv::Point2f>& t, float alpha) {
    // Calculate bounding rectangles for each triangle
    cv::Rect r1 = cv::boundingRect(t1);
    cv::Rect r2 = cv::boundingRect(t2);
    cv::Rect r = cv::boundingRect(t);

    std::vector<cv::Point2f> t1Rect, t2Rect, tRect;
    // Adjust triangle coordinates to the bounding rectangle
    for (int i = 0; i < 3; i++) {
        t1Rect.push_back(cv::Point2f(t1[i].x - r1.x, t1[i].y - r1.y));
        t2Rect.push_back(cv::Point2f(t2[i].x - r2.x, t2[i].y - r2.y));
        tRect.push_back(cv::Point2f(t[i].x - r.x, t[i].y - r.y));
    }

    // Crop the image patches corresponding to the triangles
    cv::Mat img1Rect = img1(r1);
    cv::Mat img2Rect = img2(r2);

    // Warp the image patches to the destination triangle
    cv::Mat warpImage1 = apply_affine_transform(img1Rect, t1Rect, tRect, r.size());
    cv::Mat warpImage2 = apply_affine_transform(img2Rect, t2Rect, tRect, r.size());

    // Blend the warped images using alpha
    cv::Mat imgRect = (1.0 - alpha) * warpImage1 + alpha * warpImage2;
    cv::Mat mask = cv::Mat::zeros(r.height, r.width, CV_8UC1);

    // Create a mask for the triangle
    std::vector<cv::Point> tRectInt;
    for (auto& pt : tRect) {
        tRectInt.push_back(cv::Point(static_cast<int>(pt.x), static_cast<int>(pt.y)));
    }

    cv::fillConvexPoly(mask, tRectInt, cv::Scalar(255), 16, 0);

    // Place the blended image patch back into the output image
    cv::Mat imgROI = img(r);
    imgRect.copyTo(imgROI, mask);
}

// Function to morph two images using specified correspondences and triangles
cv::Mat morph_images(cv::Mat& img1, cv::Mat& img2, std::vector<cv::Point2f>& points1, std::vector<cv::Point2f>& points2, std::vector<std::tuple<int, int, int>>& tri_list, float alpha) {
    // Create a new image to hold the morphed result
    cv::Mat morphed_img = cv::Mat::zeros(img1.size(), img1.type());

    // Compute the interpolated points
    std::vector<cv::Point2f> points(points1.size());
    for (size_t i = 0; i < points1.size(); i++) {
        points[i] = cv::Point2f((1 - alpha) * points1[i].x + alpha * points2[i].x,
            (1 - alpha) * points1[i].y + alpha * points2[i].y);
    }

    // Morph each triangle in the list
    for (auto& triangle : tri_list) {
        std::vector<cv::Point2f> t1 = { points1[std::get<0>(triangle)], points1[std::get<1>(triangle)], points1[std::get<2>(triangle)] };
        std::vector<cv::Point2f> t2 = { points2[std::get<0>(triangle)], points2[std::get<1>(triangle)], points2[std::get<2>(triangle)] };
        std::vector<cv::Point2f> t = { points[std::get<0>(triangle)], points[std::get<1>(triangle)], points[std::get<2>(triangle)] };

        morph_triangle(img1, img2, morphed_img, t1, t2, t, alpha);
    }

    return morphed_img;
}
