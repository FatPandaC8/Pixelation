#include <opencv2/opencv.hpp>
#include "block_averaging.hpp"
#include "quantize.hpp"
#include "dithering.hpp"
#include <thread>
#include "crow.h"
#include <string>
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

cv::Mat pixelateImage(const cv::Mat& input) {
    cv::Mat output;
    cv::resize(input, output, cv::Size(), 0.1, 0.1, cv::INTER_LINEAR);
    cv::resize(output, output, input.size(), 0, 0, cv::INTER_NEAREST);
    output = block(output, 16);
    output = quantize(output, 4);
    dither(output);
    return output;
}

int main() {
    crow::SimpleApp app;

    CROW_ROUTE(app, "/health")([]() {
        return crow::response(200, "Ok");
    });

    CROW_ROUTE(app, "/pixelate").methods("POST"_method)(
        [](const crow::request& req) {
            auto contentType = req.get_header_value("Content-Type");
            if (contentType.find("image/") == std::string::npos) {
                return crow::response(400, "Invalid content type");
            }

            fs::create_directories("uploads");

            std::string inputPath = "uploads/uploaded.jpg";
            std::ofstream out(inputPath, std::ios::binary);
            out.write(req.body.c_str(), req.body.size());
            out.close();

            cv::Mat img = cv::imread(inputPath);
            if (img.empty()) {
                return crow::response(400, "Failed to read uploaded image.");
            }

            cv::Mat result = pixelateImage(img);

            std::string outputPath = "uploads/pixelated_uploaded.jpg";
            cv::imwrite(outputPath, result);

            crow::json::wvalue res;
            res["message"] = "Image pixelated successfully";
            res["download_url"] = "/download/pixelated_uploaded.jpg";
            return crow::response(res);
        }
    );

    // Serve uploaded files (GET /download/<filename>)
    CROW_ROUTE(app, "/download/<string>")
        ([](const std::string& filename) {
            std::string path = "uploads/" + filename;
            if (!fs::exists(path)) {
                return crow::response(404, "File not found");
            }

            std::ifstream file(path, std::ios::binary);
            std::ostringstream buffer;
            buffer << file.rdbuf();
            file.close();

            crow::response res;
            res.code = 200;
            res.body = buffer.str();
            res.add_header("Content-Type", "image/jpeg");
            return res;
        }
    );

    app.port(8081).multithreaded().run();
    return 0;
}