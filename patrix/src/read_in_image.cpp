#include "read_in_image.h"

void image::readImage(string imageFile) {
    std::string image_path = samples::findFile(imageFile);

    //Image is stored here (img)
    Mat img = imread(image_path, IMREAD_COLOR);

    if(img.empty())
    {
        std::cout << "Could not read the image: " << image_path << std::endl;
        return;
    }

    //You can test to see the image pop up in a display by pressing 's'
    imshow("Display window", img);
    int k = waitKey(0); // Wait for a keystroke in the window

    if(k == 's')
    {
        imwrite(imageFile, img);
    }

    //RGB color matrix
    rgbMatrix(img.rows, img.cols, CV_8UC3);

    // Iterate through the image and extract RGB values
    for (int i = 0; i < img.rows; i++) {
        for (int j = 0; j < img.cols; j++) {
            Vec3b pixel = img.at<Vec3b>(i, j);
            rgbMatrix.at<Vec3b>(i, j) = pixel;
        }
    }

    /*

    //Example: How to access RGB values in the matrix (BGR order)
    int row = 10;
    int col = 20;
    Vec3b pixel = rgbMatrix.at<Vec3b>(row, col);
    int blue = pixel[0];
    int green = pixel[1];
    int red = pixel[2];

    */
}
