#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/opencv.hpp>
using namespace std;
#pragma once

cv::Mat readImage(string imagePath, int newWidth, int newHeight);
void displayImage(string imagePath, cv::Mat &img);
