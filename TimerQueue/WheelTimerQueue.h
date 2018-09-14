/* ***********************************************
MYID   : Chen Fan
LANG   : G++
PROG   : WHEELTIMERQUEUE_H
************************************************ */

#ifndef WHEELTIMERQUEUE_H
#define WHEELTIMERQUEUE_H

#include "BaseTimerQueue.h"
#include "../ThreadPool/ThreadPool.h"

#define SECOND 1000
#define MINUTE 60000
#define HOUR 3600000

struct TimerNode
{
    Timer task;
    TimerNode* next;
};

class WheelTimerQueue: public BaseTimerQueue
{
public:
    WheelTimerQueue(ThreadPool& pool);
    ~WheelTimerQueue();

    void start_Engine(int interval_ms = 50);
    void stop_Engine(bool force = false);
    bool check_Engine();

    void add_Timer(int wait_time_ms, std::function<void()> func);

private:
    void flush_queue();
    void update_queue(TimerNode& from, std::vector<TimerNode>& to);
    void add_Timer_to(std::function<void()> task, int next_time, TimerNode& to);

    ThreadPool& pool_;
    int interval_ms_;

    std::mutex wheel_mutex_;
    std::vector<TimerNode> l1_, l2_, l3_;
    int i1_, i2_, i3_;
    int total_count_;

    std::mutex engine_mutex_;
    std::thread* engine_thread_;
    eStatus engine_status_;
};

#endif // !WHEELTIMERQUEUE_H