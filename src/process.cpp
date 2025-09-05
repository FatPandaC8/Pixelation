#include "process.hpp"
#include "block_averaging.hpp"
#include "quantize.hpp"
#include "dithering.hpp"

cv::Mat processImage(const cv::Mat &image, int blockSize, int colors, int ditherStrength) {
    //TODO: need guardrails
    
    cv::Mat pixelated = block(image, blockSize);
    cv::Mat quantization = quantize(pixelated, colors);
    dither(quantization, ditherStrength);
    return quantization;
}