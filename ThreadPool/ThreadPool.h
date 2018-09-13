/* ***********************************************
MYID	: Chen Fan
LANG	: G++
PROG	: THREADPOOL_H
************************************************ */

#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <queue>
#include <future>

#define RETURN_TYPE typename std::result_of<Function(Args...)>::type

class ThreadPool
{
public:
    ThreadPool(int num_threads);
    ~ThreadPool();

    template<class Function, class... Args>
    std::future<RETURN_TYPE> add_task(Function&& f, Args&&... args);

    void wait();

private:
    friend class ThreadRunner;

    std::vector<std::thread> runners_;
    std::queue<std::function<void()>> tasks_;

    std::mutex tasks_mutex_;
    std::condition_variable cv_;
    bool stop_;
};

template<class Function, class... Args>
std::future<RETURN_TYPE> ThreadPool::add_task(Function&& f, Args&&... args)
{
    auto task = std::make_shared<std::packaged_task<RETURN_TYPE()>>(
        std::bind(std::forward<Function>(f), std::forward<Args>(args)...));
    
    std::future<RETURN_TYPE> res = task->get_future();

    {
        std::unique_lock<std::mutex> lock(tasks_mutex_);

        if (stop_) throw std::runtime_error("ThreadPool is stopped");

        tasks_.emplace([task](){(*task)();});
    }

    cv_.notify_one();

    return res;
}

#endif // !THREADPOOL_H