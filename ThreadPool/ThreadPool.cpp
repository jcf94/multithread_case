/* ***********************************************
MYID   : Chen Fan
LANG   : G++
PROG   : THREADPOOL
************************************************ */

#include "ThreadPool.h"

ThreadPool::ThreadPool(int num_threads)
    : stop_(false)
{
    for (int i=0;i<num_threads;i++)
        runners_.emplace_back([this]()
            {
                for (;;)
                {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(this->tasks_mutex_);
                        this->cv_.wait(lock,
                            [this]{return this->stop_ || !this->tasks_.empty();});
                        
                        if (this->stop_ && this->tasks_.empty()) return;
                        task = std::move(this->tasks_.front());
                        this->tasks_.pop();
                    }
                    task();
                }
            });
}

ThreadPool::~ThreadPool()
{
    {
        std::unique_lock<std::mutex> lock(tasks_mutex_);
        stop_ = true;
    }
    cv_.notify_all();
    for (auto &i:runners_) i.join();
}

void ThreadPool::wait()
{
    while (!tasks_.empty()) {}
}