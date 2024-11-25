#include "read_in_image.h"
#include <utility>

cv::Mat image::readImage(string file) {
    this->imageFile = file;
    std::string image_path = cv::samples::findFile(imageFile);
    //Image is stored here (img)
    img = cv::imread(image_path, cv::IMREAD_COLOR);

    cv::Size newSize(848, 476);
    cv::resize(img, img, newSize, 0, 0, cv::INTER_LINEAR);

    return img;
}

void image::displayImage() {
    imshow("Display window", img);
    int k = cv::waitKey(0); // Wait for a keystroke in the window
    if(k == 's')
    {
        imwrite(imageFile, img);
    }
}
