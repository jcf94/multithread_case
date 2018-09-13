/* ***********************************************
MYID   : Chen Fan
LANG   : G++
PROG   : HEAPTIMERQUEUE
************************************************ */

#include <stdio.h>

#include "HeapTimerQueue.h"

HeapTimerQueue::HeapTimerQueue(ThreadPool& pool)
    : pool_(pool), engine_thread_(NULL), engine_status_(IDLE) {}

HeapTimerQueue::~HeapTimerQueue()
{
    if (engine_status_ != IDLE) stop_Engine(true);
}

void HeapTimerQueue::start_Engine()
{
    if (engine_status_ != IDLE)
    {
        throw std::runtime_error("Engine Already Started.");
        return;
    }

    std::lock_guard<std::mutex> lock(engine_mutex_);
    engine_status_ = WORKING;
    engine_thread_ = new std::thread([this]()
        {
            for (;;)
            {
                this->flush_queue();
                if (!this->check_Engine()) break;
                std::this_thread::sleep_for(std::chrono::milliseconds(5));
            }
        });
}

void HeapTimerQueue::stop_Engine(bool force)
{
    if (engine_status_ != WORKING)
    {
        throw std::runtime_error("Engine Already Stopped");
        return ;
    }

    {
        std::lock_guard<std::mutex> lock(engine_mutex_);
        if (force)
        {
            printf("Force Stopping Engine\n");
            engine_status_ = IDLE;
        } else
        {
            printf("Engine Waiting to Stop\n");
            engine_status_ = ENDING;
        }
    }
    engine_thread_->join();
}

bool HeapTimerQueue::check_Engine()
{
    std::lock_guard<std::mutex> lock(engine_mutex_);

    if (engine_status_ == IDLE) return false;
    if (engine_status_ == ENDING && list_.empty())
    {
        engine_status_ = IDLE;
        return false;
    }
    return true;
}

void HeapTimerQueue::flush_queue()
{
    std::lock_guard<std::mutex> lock(list_mutex_);
    while (!list_.empty())
    {
        int64_t current_time = Clock::get_CurrentTime();
        const Timer& now = list_.top();
        if (now.time <= current_time)
        {
            pool_.add_task(now.func);
            list_.pop();
        } else break;
    }
}

void HeapTimerQueue::add_Timer(int wait_time, std::function<void()> func)
{
    if (engine_status_ != ENDING)
    {
        int64_t start_time = Clock::get_CurrentTime();

        std::lock_guard<std::mutex> lock(list_mutex_);
        list_.push({start_time + wait_time, func});
    }
}