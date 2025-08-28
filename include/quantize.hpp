#ifndef QUANTIZE_HPP
#define QUANTIZE_HPP

//2 options: median-cut + k-means
//k-means: clusters RGB vectors into k centers -> each pixel maps to the nearest center
//Input: Pixel (after block_averaging)
//Output: Quantized Pixel

#include <opencv2/opencv.hpp>
#include <iostream>
#include <random>

cv::Mat quantize(const cv::Mat &image, int K, int max_iters = 10); //default max_iter = 10
//K is number of color (3 colors = 3 centroids)

#endif