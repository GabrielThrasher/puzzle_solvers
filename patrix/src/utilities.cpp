#include "utilities.h"

int Utilities::getComplementEdge(int num) {
    string str = to_string(num);
    string comp = "";
    unordered_map<char, char> compMap = {
            {'1', '5'},
            {'2', '6'},
            {'4', '4'},
            {'6', '2'},
            {'5', '1'},
    };

    for (auto chr : str) {
        comp += compMap[chr];
    }

    return stoi(comp);
}

int Utilities::hashRGBvalues(tuple<int, int, int> rgb) {
    string hashValue = to_string(get<0>(rgb)) + to_string(get<1>(rgb)) +
                       to_string(get<2>(rgb));

    return stoi(hashValue);
}

cv::Mat Utilities::readImage(string imagePath, int newWidth, int newHeight) {
    cv::Mat img = cv::imread(imagePath, cv::IMREAD_COLOR);

    cv::Size newSize(newWidth, newHeight);
    cv::resize(img, img, newSize, 0, 0, cv::INTER_LINEAR);

    return img;
}

void Utilities::displayImage(string imagePath, cv::Mat &img) {
    imshow("Display window", img);
    int k = cv::waitKey(0); // Wait for a keystroke in the window
    if (k == 's') {
        imwrite(imagePath, img);
    }
}

