#ifndef THREAD_POOL_HPP
#define THREAD_POOL_HPP
#include <thread>
#include <queue>
#include <vector>
#include <functional>
#include <condition_variable>
#include <atomic>

class ThreadPool {
public:
    explicit ThreadPool(size_t numThreads);
    ~ThreadPool();

    void enqueue(std::function<void()> task);
    void waitToJoin();

private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    std::mutex queueMutex;
    std::condition_variable condition;
    std::atomic<bool> stop{false};
    std::atomic<int> activeTasks{0};
};

#endif