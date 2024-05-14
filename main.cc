#include "face_correspondences.h"
#include "delaunay_triangulation.h"
#include "face_morph.h"

#include <iostream>
#include <vector>
#include <tuple>
#include <opencv2/opencv.hpp>
#include <chrono>

int main() {
    // Path to input images
    std::string pathToFirstImage = "img/bradpitt.png";
    std::string pathToSecondImage = "img/ld2.png";

    // Start the clock for the entire process
    auto startTotal = std::chrono::high_resolution_clock::now();

    // Measure time for generating face correspondences
    auto start = std::chrono::high_resolution_clock::now();
    auto data = generate_face_correspondences(pathToFirstImage, pathToSecondImage);
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;
    std::cout << "Time for generating face correspondences: " << elapsed.count() << " seconds." << std::endl;

    // Optional: Uncomment to display the cropped images for verification
    // cv::imshow("Cropped Image 1", data.cropped1);
    // cv::imshow("Cropped Image 2", data.cropped2);
    // cv::waitKey(0);  // Wait for a key press

    // Measure time for Delaunay triangulation
    start = std::chrono::high_resolution_clock::now();
    std::vector<std::tuple<int, int, int>> delaunayTriangles = makeDelaunay(data.size[1], data.size[0], data.corresp);
    finish = std::chrono::high_resolution_clock::now();
    elapsed = finish - start;
    std::cout << "Time for Delaunay triangulation: " << elapsed.count() << " seconds." << std::endl;

    // Measure time for morphing images
    start = std::chrono::high_resolution_clock::now();
    cv::Mat result = morph_images(data.cropped1, data.cropped2, data.list1, data.list2, delaunayTriangles, 0.5);
    finish = std::chrono::high_resolution_clock::now();
    elapsed = finish - start;
    std::cout << "Time for morphing images: " << elapsed.count() << " seconds." << std::endl;

    // Save the morphed image to a file
    std::string outputPath = "img/morphed_image.jpg";
    cv::imwrite(outputPath, result);
    std::cout << "Morphed image saved to: " << outputPath << std::endl;

    // Measure total elapsed time
    auto finishTotal = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> totalElapsed = finishTotal - startTotal;
    std::cout << "Total elapsed time: " << totalElapsed.count() << " seconds." << std::endl;

    return 0;
}

