#pragma once
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/opencv.hpp>
#include <chrono>
using namespace std;

class Utilities {
    unordered_map<string, chrono::time_point<chrono::system_clock>> sectionTimes;
    string sectionDivder = "------------------------------------------------";
public:
    int getComplementEdge(int num);
    int hashRGBvalues(tuple<int, int, int> rgb);

    cv::Mat readImage(string imagePath, int newWidth, int newHeight);
    void displayImage(string imagePath, cv::Mat &img);

    void startSectionTime(string section);
    int endSectionTime(string section, string unit="milliseconds");

    void Utilities::displayHeader(string section);
    void Utilities::displaySectionDivder();
    void Utilities::displayText(string text);
};