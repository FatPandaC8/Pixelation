#include "load.hpp"
#include "block_averaging.hpp"
#include "thread_pool.hpp"
#include "quantize.hpp"
#include "dithering.hpp"

int main() {
    std::vector<std::string> imagesPath = {"assets/apple.jpg", "assets/banana.jpg"};
    std::vector<cv::Mat> loadedImages; //shared vector to store loaded images

    std::cout << "Choose loading mode:\n";
    std::cout << "1. Thread-per-image\n";
    std::cout << "2. Thread-pool\n";
    std::cout << "> ";

    int choice;
    std::cin >> choice;

    if(choice == 1) {
        std::cout<< "[Mode] Thread-per-image\n";
        std::vector<std::thread> threads;

        for(const std::string&path : imagesPath) {
            threads.emplace_back(loadAndPushImage, path, std::ref(loadedImages));
        }

        for(auto &t : threads) {
            if(t.joinable()) {
                t.join();
            }
        }
    }
    else if(choice == 2) {
        std::cout << "[Mode] Thread-pool\n";
        size_t numThreads = std::thread::hardware_concurrency();
        if(numThreads == 0) {
            numThreads = 4;
        }

        ThreadPool pool(numThreads);

        for(const auto &path : imagesPath) {
            pool.enqueue([&loadedImages, path] {
                loadAndPushImage(path, loadedImages);
            }); 
        }
    }
    else {
        std::cerr << "Invalid choice\n";
        return -1;
    }

    int blockSize = 16;
    int index = 0;

    for (const auto &img : loadedImages) {
        if (img.empty()) {
            std::cerr << "Failed to load image!\n";
            return -1;
        }

        try {
            cv::Mat pixelated = block(img, blockSize);
            cv::Mat quantization = quantize(pixelated, 16);
            dither(quantization, 4);

            std::string name = std::to_string(index++);
            cv::imshow(name, quantization);
        }
        catch (const std::exception &e) {
            std::cerr << e.what() << '\n';
            return -1;
        }
    }

    cv::waitKey(0);
    cv::destroyAllWindows();

    return 0;
}