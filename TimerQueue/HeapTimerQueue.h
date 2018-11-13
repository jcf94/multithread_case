/* ***********************************************
MYID   : Chen Fan
LANG   : G++
PROG   : HEAPTIMERQUEUE_H
************************************************ */

#ifndef HEAPTIMERQUEUE_H
#define HEAPTIMERQUEUE_H

#include <queue>
#include <mutex>
#include <thread>

#include "BaseTimerQueue.h"
#include "../ThreadPool/src/ThreadPool.h"

struct cmp
{
    bool operator()(Timer a, Timer b)
    {
        return a.time > b.time;
    }
};

class HeapTimerQueue: public BaseTimerQueue
{
public:
    HeapTimerQueue(ThreadPool& pool);
    ~HeapTimerQueue();

    void start_Engine(int interval_ms = 50);
    void stop_Engine(bool force = false);
    bool check_Engine();

    void add_Timer(int wait_time_ms, std::function<void()> func);

private:
    void flush_queue();

    ThreadPool& pool_;

    std::mutex list_mutex_;
    std::priority_queue<Timer, std::vector<Timer>, cmp> list_;

    std::mutex engine_mutex_;
    std::thread* engine_thread_;
    eStatus engine_status_;
};

#endif // !HEAPTIMERQUEUE_H