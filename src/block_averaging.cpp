#include "block_averaging.hpp"
#include <algorithm>

cv::Mat block(const cv::Mat &image, int blockSize) {
    cv::Mat output = image.clone();

    int rows = image.rows;
    int cols = image.cols;

    for(int y = 0; y < rows; y += blockSize) { //why y is blockSize jumps not 
        for(int x = 0; x < cols; x += blockSize) {
            int blockWidth = std::min(blockSize, cols - x);
            int blockHeight = std::min(blockSize, rows - y);

            cv::Rect blockRect(x, y, blockWidth, blockHeight);
            cv::Mat block = image(blockRect);

            cv::Scalar avgColor = cv::mean(block);

            cv::rectangle(output, blockRect, avgColor, cv::FILLED);
        }
    }

    return output;
}