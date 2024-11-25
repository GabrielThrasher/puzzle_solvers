#include "read_in_image.h"

#include <utility>

Mat image::readImage(string File) {
    this->imageFile = File;
    std::string image_path = samples::findFile(imageFile);
    //Image is stored here (img)
    img = imread(image_path, IMREAD_COLOR);

    cv::Size newSize(848, 476);
    cv::resize(img, img, newSize, 0, 0, cv::INTER_LINEAR);

    return img;
}

void image::displayImage() {
    imshow("Display window", img);
    int k = waitKey(0); // Wait for a keystroke in the window
    if(k == 's')
    {
        imwrite(imageFile, img);
    }
}
