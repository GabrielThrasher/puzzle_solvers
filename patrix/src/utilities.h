#pragma once
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/opencv.hpp>
#include "puzzle.h"
using namespace std;

class Utilities {
public:
    int getComplementEdge(int num);

    int hashRGBvalues(tuple<int, int, int> rgb);

    cv::Mat readImage(string imagePath, int newWidth, int newHeight);
    void displayImage(string imagePath, cv::Mat &img);

};