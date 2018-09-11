/* ***********************************************
MYID   : Chen Fan
LANG   : G++
PROG   : THREADRUNNER
************************************************ */

#include "ThreadRunner.h"

void ThreadRunner::operator()()
{
    std::function<void()> task;

    while (true)
    {
        {
            std::unique_lock<std::mutex> lock(pool_.tasks_mutex_);

            while (!pool_.stop_ && pool_.tasks_.empty())
            {
                pool_.cv_.wait(lock);
            }

            if (pool_.stop_) return;

            task = pool_.tasks_.front();
            pool_.tasks_.pop();
        }

        task();
    }
}