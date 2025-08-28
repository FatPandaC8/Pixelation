#ifndef DITHERING_HPP
#define DITHERING_HPP

#include <opencv2/opencv.hpp>
#include <algorithm>

void dither(cv::Mat &image, int levels = 2);
inline float findClosestPaletteColor(float oldPixel, int levels = 2);

#endif