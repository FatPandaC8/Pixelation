#include <opencv2/opencv.hpp>
#include <algorithm>

inline float findClosestPaletteColor(float oldPixel, int levels = 2) {
    int step = 255 / (levels - 1);
    return cvRound(oldPixel / step) * step;
}

void dither(cv::Mat &image, int levels = 2) {
    image.convertTo(image, CV_32F);

    for (int y = 0; y < image.rows - 1; y++) {
        for (int x = 1; x < image.cols - 1; x++) {
            cv::Vec3f &pixel = image.at<cv::Vec3f>(y, x);
            for (int c = 0; c < 3; c++) {
                float oldPixel = pixel[c];
                float newPixel = findClosestPaletteColor(oldPixel, levels);
                pixel[c] = newPixel;

                float quantError = oldPixel - newPixel;

                // Diffuse error to neighbors safely
                image.at<cv::Vec3f>(y, x + 1)[c] =
                    std::max(0.f, std::min(255.f, image.at<cv::Vec3f>(y, x + 1)[c] + quantError * 7.f / 16.f));

                image.at<cv::Vec3f>(y + 1, x - 1)[c] =
                    std::max(0.f, std::min(255.f, image.at<cv::Vec3f>(y + 1, x - 1)[c] + quantError * 3.f / 16.f));

                image.at<cv::Vec3f>(y + 1, x)[c] =
                    std::max(0.f, std::min(255.f, image.at<cv::Vec3f>(y + 1, x)[c] + quantError * 5.f / 16.f));

                image.at<cv::Vec3f>(y + 1, x + 1)[c] =
                    std::max(0.f, std::min(255.f, image.at<cv::Vec3f>(y + 1, x + 1)[c] + quantError * 1.f / 16.f));
            }
        }
    }

    image.convertTo(image, CV_8U);
}
