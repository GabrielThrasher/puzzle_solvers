#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
using namespace std;
using namespace cv;

class image {
private:
    Mat img;
    string imageFile;
public:
    Mat readImage(string imageFile);

    void displayImage();
};

