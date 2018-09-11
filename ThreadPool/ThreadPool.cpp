/* ***********************************************
MYID   : Chen Fan
LANG   : G++
PROG   : THREADPOOL
************************************************ */

#include "ThreadRunner.h"

ThreadPool::ThreadPool(int num_threads)
    : stop_(false)
{
    for (int i=0;i<num_threads;i++)
        runners_.push_back(std::thread(ThreadRunner(*this)));
}

ThreadPool::~ThreadPool()
{
    stop_ = true;
    cv_.notify_all();
    for (auto &i:runners_)
        i.join();
}

void ThreadPool::wait()
{
    while (!tasks_.empty()) {}
}