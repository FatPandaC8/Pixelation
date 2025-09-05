#pragma once
#include <opencv2/opencv.hpp>

cv::Mat processImage(const cv::Mat &image, int blockSize, int colors, int ditherStrength);