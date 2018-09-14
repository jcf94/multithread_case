/* ***********************************************
MYID   : Chen Fan
LANG   : G++
PROG   : WHEELTIMERQUEUE
************************************************ */

#include "WheelTimerQueue.h"

WheelTimerQueue::WheelTimerQueue(ThreadPool& pool)
    : pool_(pool), engine_thread_(NULL), engine_status_(IDLE), total_count_(0) {}

WheelTimerQueue::~WheelTimerQueue()
{
    if (engine_status_ != IDLE) stop_Engine(true);
}

void WheelTimerQueue::start_Engine(int interval_ms)
{
    if (engine_status_ != IDLE)
    {
        throw std::runtime_error("Engine Already Started.");
        return;
    }

    {
        std::lock_guard<std::mutex> lock(wheel_mutex_);
        interval_ms_ = interval_ms;
        for (int i=0;i<1000/interval_ms;i++) l1_.push_back({{0, NULL}, NULL});
        for (int i=0;i<60;i++) l2_.push_back({{0, NULL}, NULL});
        for (int i=0;i<60;i++) l3_.push_back({{0, NULL}, NULL});
        i1_ = 0; i2_ = 0; i3_ = 0;
    }
    std::lock_guard<std::mutex> lock(engine_mutex_);
    engine_status_ = WORKING;
    engine_thread_ = new std::thread([this, interval_ms]()
        {
            for (;;)
            {
                this->pool_.add_task([this](){this->flush_queue();});
                if (!this->check_Engine()) break;
                std::this_thread::sleep_for(std::chrono::milliseconds(interval_ms));
            }
        });
}

void WheelTimerQueue::stop_Engine(bool force)
{
    if (engine_status_ != WORKING)
    {
        throw std::runtime_error("Engine Already Stopped");
        return;
    }
    {
        std::lock_guard<std::mutex> lock(wheel_mutex_);
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

bool WheelTimerQueue::check_Engine()
{
    std::lock_guard<std::mutex> lock(engine_mutex_);
    if (engine_status_ == IDLE) return false;
    if (engine_status_ == ENDING && total_count_ == 0)
    {
        engine_status_ = IDLE;
        return false;
    }
    return true;
}

void WheelTimerQueue::flush_queue()
{
    std::lock_guard<std::mutex> lock(wheel_mutex_);

    TimerNode* now = l1_[i1_].next;
    while (now)
    {
        pool_.add_task(now->task.func);
        TimerNode* last = now;
        now = now->next;
        delete(last);
        total_count_--;
    }
    l1_[i1_].next = NULL;

    i1_++;
    if (i1_ == l1_.size())
    {
        i1_ = 0; i2_ ++;
        if (i2_ == l2_.size())
        {
            i2_ = 0; i3_ ++;
            if (i3_ == l3_.size()) i3_ = 0;
            update_queue(l3_[i3_], l2_);
        }
        update_queue(l2_[i2_], l1_);
    }
    //printf("%d %d %d\n", i1_, i2_, i3_);
}

void WheelTimerQueue::update_queue(TimerNode& from, std::vector<TimerNode>& to)
{
    TimerNode* now = from.next;
    if (&to == &l1_)
    {
        while (now)
        {
            int pos = now->task.time / interval_ms_ + i1_;
            pos %= l1_.size();
            add_Timer_to(now->task.func, 0, l1_[pos]);
            TimerNode* last = now;
            now = now->next;
            delete(last);
        }
    } else
    {
        while (now)
        {
            int pos = now->task.time / SECOND + i2_;
            pos %= l2_.size();
            add_Timer_to(now->task.func, now->task.time%SECOND, l2_[pos]);
            TimerNode* last = now;
            now = now->next;
            delete(last);
        }
    }
}

void WheelTimerQueue::add_Timer(int wait_time_ms, std::function<void()> func)
{
    if (engine_status_ != ENDING)
    {
        std::lock_guard<std::mutex> lock(wheel_mutex_);
        if (wait_time_ms < SECOND)
        {
            int pos = wait_time_ms / interval_ms_ + i1_;
            pos %= l1_.size();
            add_Timer_to(func, 0, l1_[pos]);
            total_count_++;
        } else if (wait_time_ms < MINUTE)
        {
            int pos = wait_time_ms / SECOND + i2_;
            pos %= l2_.size();
            add_Timer_to(func, wait_time_ms%SECOND, l2_[pos]);
            total_count_++;
        } else if (wait_time_ms < HOUR)
        {
            int pos = wait_time_ms / MINUTE + i3_;
            pos %= l3_.size();
            add_Timer_to(func, wait_time_ms%MINUTE,l3_[pos]);
            total_count_++;
        } else
        {
            printf("Too long. Error\n");
        }
    }
}

void WheelTimerQueue::add_Timer_to(std::function<void()> task, int next_time, TimerNode& to)
{
    TimerNode* now = &to;
    while (now->next) now = now->next;
    TimerNode* next = new TimerNode({{0, task}, NULL});
    now->next = next;
}