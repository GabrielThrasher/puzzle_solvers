#include "process_image.h"
#include <utility>

cv::Mat readImage(string filePath, int newWidth, int newHeight) {
    cv::Mat img = cv::imread(filePath, cv::IMREAD_COLOR);

    cv::Size newSize(newWidth, newHeight);
    cv::resize(img, img, newSize, 0, 0, cv::INTER_LINEAR);

    return img;
}

void displayImage(string imagePath, cv::Mat &img) {
    imshow("Display window", img);
    int k = cv::waitKey(0); // Wait for a keystroke in the window
    if (k == 's') {
        imwrite(imagePath, img);
    }
}
