#pragma once

#include <string>
#include <functional>
#include <future>
#include <queue>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <vector>

class AsyncPipeline {
public:
    AsyncPipeline(size_t threads);
    ~AsyncPipeline();

    template<class F, class... Args>
    auto enqueue(F&& f, Args&&... args)
        -> std::future<typename std::result_of<F(Args...)>::type>;

private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;

    std::mutex queue_mutex;
    std::condition_variable condition;
    bool stop;
};

template<class F, class... Args>
auto AsyncPipeline::enqueue(F&& f, Args&&... args)
    -> std::future<typename std::result_of<F(Args...)>::type> {
    using return_type = typename std::result_of<F(Args...)>::type;

    auto task = std::make_shared<std::packaged_task<return_type()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
        );

    std::future<return_type> res = task->get_future();
    {
        std::unique_lock<std::mutex> lock(queue_mutex);

        if(stop)
            throw std::runtime_error("enqueue on stopped AsyncPipeline");

        tasks.emplace([task](){ (*task)(); });
    }
    condition.notify_one();
    return res;
}
