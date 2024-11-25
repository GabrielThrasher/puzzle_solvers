#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
using namespace std;
#pragma once

class image {
    cv::Mat img;
    string imageFile;
public:
    cv::Mat readImage(string file);
    void displayImage();
};

