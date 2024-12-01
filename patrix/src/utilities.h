#pragma once
#include <chrono>
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/opencv.hpp>
using namespace std;

namespace Utilities {
int getComplementEdge(int num);
int hashRGBvalues(tuple<int, int, int> rgb);

cv::Mat readImage(string imagePath, int newWidth, int newHeight);
void displayImage(string imagePath, cv::Mat &img);

void startSectionTime(string section);
int endSectionTime(string section, string unit = "milliseconds");

void displayHeader(string section);
void displaySectionDivder();
void displayText(string text);
}; // namespace Utilities