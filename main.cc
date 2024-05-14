#include "face_correspondences.h"
#include "delaunay_triangulation.h"
#include "face_morph.h"

#include <iostream>
#include <vector>
#include <tuple>
#include <opencv2/opencv.hpp>
#include <chrono>

int main() {
    // Start the clock
    auto start = std::chrono::high_resolution_clock::now();

    // Path to input images
    std::string pathToFirstImage = "img/bradpitt.png";
    std::string pathToSecondImage = "img/ld2.png";

    // Generate face correspondences between two images
    auto data = generate_face_correspondences(pathToFirstImage, pathToSecondImage);

    // Optional: Uncomment to display the cropped images for verification
    cv::imshow("Cropped Image 1", data.cropped1);
    cv::imshow("Cropped Image 2", data.cropped2);
    cv::waitKey(0);  // Wait for a key press

    // Perform Delaunay triangulation on the averaged points
    std::vector<std::tuple<int, int, int>> delaunayTriangles = makeDelaunay(data.size[1], data.size[0], data.corresp);

    // Morph the images using a blend factor of 0.5
    cv::Mat result = morph_images(data.cropped1, data.cropped2, data.list1, data.list2, delaunayTriangles, 0.5);

    // Save the morphed image to a file
    std::string outputPath = "img/morphed_image.jpg";
    cv::imwrite(outputPath, result);
    std::cout << "Morphed image saved to: " << outputPath << std::endl;

    // Stop the clock and calculate the elapsed time
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;
    std::cout << "Elapsed time: " << elapsed.count() << " seconds." << std::endl;

    return 0;
}
