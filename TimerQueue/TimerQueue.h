/* ***********************************************
MYID   : Chen Fan
LANG   : G++
PROG   : TIMERQUEUE_H
************************************************ */

#ifndef TIMERQUEUE_H
#define TIMERQUEUE_H

#include <queue>
#include <mutex>
#include <thread>

#include "BaseTimerQueue.h"
#include "../ThreadPool/ThreadPool.h"

enum eStatus
{
    IDLE, WORKING, ENDING
};

class TimerQueue: public BaseTimerQueue
{
public:
    TimerQueue(ThreadPool& pool);
    ~TimerQueue();

    void start_Engine();
    void stop_Engine(bool force = false);
    bool check_Engine();

    void flush_queue();

    void add_Timer(int wait_time, std::function<void()> func);

private:
    ThreadPool& pool_;

    std::mutex list_mutex_;
    std::priority_queue<Timer> list_;

    std::mutex engine_mutex_;
    std::thread* engine_thread_;
    eStatus engine_status_;
};

#endif // !TIMERQUEUE_H