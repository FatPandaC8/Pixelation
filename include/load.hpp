#ifndef LOAD_HPP
#define LOAD_HPP

#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <thread>
#include <mutex>

cv::Mat loadImage(const std::string &imagePath); //a matrix of color in order of (B,G,R)
void printImageInfo(const cv::Mat &image);
void loadAndPushImage(const std::string &path, std::vector<cv::Mat> &loadedImages);

#endif