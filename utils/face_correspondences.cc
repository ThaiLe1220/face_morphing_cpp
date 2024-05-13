#include "face_correspondences.h"

// Generates data regarding face correspondences between two images using Dlib's face detection and landmark prediction.
FaceCorrespondenceData generate_face_correspondences(const std::string& filename1, const std::string& filename2) {
    dlib::frontal_face_detector detector = dlib::get_frontal_face_detector(); // Load Dlib's face detector
    dlib::shape_predictor predictor;
    dlib::deserialize("shape_predictor_68_face_landmarks.dat") >> predictor; // Load Dlib's shape predictor
    std::vector<cv::Point2f> corresp(68, cv::Point2f(0.0f, 0.0f)); // Initialize correspondence vector with cv::Point2f
    std::vector<std::vector<cv::Point2f>> lists(2); // Lists to hold landmarks for each image

    cv::Mat cropped1, cropped2;
    cv::Mat cvImg1 = cv::imread(filename1);
    cv::Mat cvImg2 = cv::imread(filename2);
    crop_image(cvImg1, cvImg2, cropped1, cropped2);
    std::vector<cv::Mat> imgList = { cropped1,cropped2 };
    std::vector<int> size = { cropped1.rows, cropped1.cols };

    std::vector<cv::Point2f> extra_points = {
        cv::Point2f(1.0f, 1.0f),
        cv::Point2f(static_cast<float>(size[1] - 1), 1.0f),
        cv::Point2f(static_cast<float>(size[1] / 2), 1.0f),
        cv::Point2f(1.0f, static_cast<float>(size[0] - 1)),
        cv::Point2f(1.0f, static_cast<float>(size[0] / 2)),
        cv::Point2f(static_cast<float>(size[1] / 2), static_cast<float>(size[0] - 1)),
        cv::Point2f(static_cast<float>(size[1] - 1), static_cast<float>(size[0] - 1)),
        cv::Point2f(static_cast<float>(size[1] - 1), static_cast<float>(size[0] / 2))
    };

    for (int idx = 0; idx < imgList.size(); ++idx) {
        auto& img = imgList[idx];
        auto& currList = lists[idx];

        dlib::cv_image<dlib::bgr_pixel> dlib_img(img); // Convert OpenCV image to Dlib format
        std::vector<dlib::rectangle> dets = detector(dlib_img, 1); // Detect faces

        if (dets.empty()) {
            print_error_message();
            continue;
        }

        for (auto& rect : dets) {
            // Predict facial landmarks
            dlib::full_object_detection shape = predictor(dlib_img, rect);

            // Store landmarks and accumulate coordinates for averaging
            for (int i = 0; i < 68; i++) {
                cv::Point2f pt(shape.part(i).x(), shape.part(i).y());
                currList.push_back(pt);
                corresp[i] += pt; // Accumulate coordinates for averaging
            }

            currList.insert(currList.end(), extra_points.begin(), extra_points.end());
        }
    }

    // Compute average coordinates of landmarks
    for (auto& point : corresp) {
        point.x /= imgList.size();
        point.y /= imgList.size();
    }

    corresp.insert(corresp.end(), extra_points.begin(), extra_points.end()); // Add extra points to the final correspondence list

    FaceCorrespondenceData result;
    result.size = size;
    result.cropped1 = cropped1;
    result.cropped2 = cropped2;
    result.list1 = lists[0];
    result.list2 = lists[1];
    result.corresp = corresp;

    return result;
}

// Print an error message if no face is detected in the image processing step
void print_error_message() {
    std::cerr << "Sorry, no face detected in the image." << std::endl;
}

// Calculates margins for cropping based on the dimensions of the input images to align them in size
void calculate_margin_help(const cv::Mat& img1, const cv::Mat& img2, std::vector<int>& results) {
    int diff0 = abs(img1.rows - img2.rows) / 2;
    int diff1 = abs(img1.cols - img2.cols) / 2;
    int avg0 = (img1.rows + img2.rows) / 2;
    int avg1 = (img1.cols + img2.cols) / 2;

    results = { img1.rows, img2.rows, img1.cols, img2.cols, diff0, diff1, avg0, avg1 };
}

// Crop two images to ensure they are of the same dimensions for further processing
void crop_image(const cv::Mat& img1, const cv::Mat& img2, cv::Mat& cropped1, cv::Mat& cropped2) {
    std::vector<int> metrics;
    calculate_margin_help(img1, img2, metrics);

    int size1_rows = metrics[0];
    int size2_rows = metrics[1];
    int size1_cols = metrics[2];
    int size2_cols = metrics[3];
    int diff0 = metrics[4];
    int diff1 = metrics[5];
    int avg0 = metrics[6];
    int avg1 = metrics[7];
    cv::Rect roi1, roi2;

    // Determine cropping areas based on relative dimensions of input images
    if (size1_rows >= size2_rows && size1_cols >= size2_cols) {
        roi1 = cv::Rect(diff1, diff0, size2_cols, size2_rows);
        cropped1 = img1(roi1);
        cropped2 = img2;
    }
    else if (size1_rows <= size2_rows && size1_cols <= size2_cols) {
        roi2 = cv::Rect(diff1, diff0, size1_cols, size1_rows);
        cropped2 = img2(roi2);
        cropped1 = img1;
    }
    else if (size1_rows >= size2_rows && size1_cols <= size2_cols) {
        roi1 = cv::Rect(0, diff0, size1_cols, size2_rows);
        roi2 = cv::Rect(diff1, 0, size1_cols, size2_rows);
        cropped1 = img1(roi1);
        cropped2 = img2(roi2);
    }
    else if (size1_rows <= size2_rows && size1_cols >= size2_cols) {
        roi1 = cv::Rect(diff1, 0, size2_cols, size1_rows);
        roi2 = cv::Rect(0, diff0, size2_cols, size1_rows);
        cropped1 = img1(roi1);
        cropped2 = img2(roi2);
    }
}
