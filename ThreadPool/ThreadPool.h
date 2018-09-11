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

class ThreadPool
{
public:
    ThreadPool(int num_threads);
    ~ThreadPool();

    template<class F>
    void add_task(F f)
    {
        {
            std::unique_lock<std::mutex> lock(tasks_mutex_);

            tasks_.push(std::function<void()>(f));
        }
        cv_.notify_one();
    }
    void wait();

private:
    friend class ThreadRunner;

    std::vector<std::thread> runners_;
    std::queue<std::function<void()>> tasks_;

    std::mutex tasks_mutex_;
    std::condition_variable cv_;
    bool stop_;
};

#endif // !THREADPOOL_H