#include "load.hpp"
//A mutex to protect shared access to the images vector
std::mutex imageMutex;
/*
    @brief: Loads an image and pushes it into a shared vector in a thread-safe manner
    @param: images - a reference to the vector where loaded images are stored
*/

cv::Mat loadImage(const std::string &imagePath) {
    cv::Mat image = cv::imread(imagePath, cv::IMREAD_COLOR);

    if (image.empty()) {
        throw std::runtime_error("Error: Could not load the image from: " + imagePath);
    }
    return image;
}

void printImageInfo(const cv::Mat &image) {
    std::cout<<"Image Info: " << '\n';
    std::cout<<"Width: " << image.cols <<" px\n";
    std::cout<<"Height: " << image.rows <<" py\n";
    std::cout<<"Channels: " << image.channels() << '\n';
}

void loadAndPushImage(const std::string &path, std::vector<cv::Mat> &loadedImages) {
    try {
        cv::Mat img = loadImage(path);

        std::lock_guard<std::mutex> lock(imageMutex); //shared one mutex for editing the loadedImages vector
        loadedImages.push_back(img);

        std::cout << "[Thread] Loaded: "<< path << '\n';
    }
    catch(const std::exception& e) {
        std::cerr << "[Thread] Failed to load: "<< e.what() <<'\n';
    }
}