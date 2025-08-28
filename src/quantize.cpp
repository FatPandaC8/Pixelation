#include "quantize.hpp"

cv::Mat quantize(const cv::Mat &image, int K, int max_iters) {
    //Prepare the data
    cv::Mat data = image.reshape(3, image.rows * image.cols); //squeeze the 3 color into 1 dimension
    data.convertTo(data, CV_32F, 1.0 / 255.0);
    /*
    CV_8U: 8 bit/pixel - a pixel can have values 0-255 
    CV_32F: float - pixel can have any value between 0-1.0 -> need to be converted into 8 bit to save
    */

    int N = data.rows; //total pixels

    //initialize random centroids
    std::vector<cv::Vec3f> centroids;

    std::mt19937 rng((unsigned)time(0)); //generate pseudo-random number; produce sequence of 32-bit integer that only repeats itself after 2^19937 - 1 number have been generated

    std::uniform_int_distribution<int> dist(0, N - 1);

    for(int i = 0; i < K; i++) {
        centroids.push_back(data.at<cv::Vec3f>(dist(rng)));
    }

    std::vector<int> labels(N, 0);

    //run k-means iteration
    for(int i = 0; i < max_iters; i++) {
        bool changed = false;

        //assign each pixel to the nearest centroid
        for(int id = 0; id < N; id++) {
            cv::Vec3f pixel = data.at<cv::Vec3f>(id); 
            float minDist = FLT_MAX;
            int clusterID = -1;

            for(int k = 0; k < K; k++) {
                cv::Vec3f diff = pixel - centroids[k];
                float euclid = diff.dot(diff);
                if(euclid < minDist) {
                    minDist = euclid;
                    clusterID = k;
                }
            }

            if (labels[id] != clusterID) {
                labels[id] = clusterID;
                changed = true;
            }
        }
        
        if(!changed) {
            break;
        }

        //update centroid
        std::vector<cv::Vec3f> newCentroids(K, cv::Vec3f(0,0,0));
        std::vector<int> counts(K,0);

        for(int i = 0; i < N; i++) {
            newCentroids[labels[i]] += data.at<cv::Vec3f>(i);
            counts[labels[i]]++;
        }

        for(int k = 0; k < K; k++) {
            if(counts[k] > 0) {
                centroids[k] = newCentroids[k] / counts[k];
            }
        }
    }

    //replace pixel with centroid color
    for(int i = 0; i < N; i++) {
        data.at<cv::Vec3f>(i) = centroids[labels[i]];
    }

    cv::Mat quantized = data.reshape(3, image.rows);
    quantized.convertTo(quantized, CV_8U, 255.0);
    return quantized;
}