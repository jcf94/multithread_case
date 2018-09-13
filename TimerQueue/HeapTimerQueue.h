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
#include "../ThreadPool/ThreadPool.h"

enum eStatus
{
    IDLE, WORKING, ENDING
};

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

    void start_Engine();
    void stop_Engine(bool force = false);
    bool check_Engine();

    void flush_queue();

    void add_Timer(int wait_time, std::function<void()> func);

private:
    ThreadPool& pool_;

    std::mutex list_mutex_;
    std::priority_queue<Timer, std::vector<Timer>, cmp> list_;

    std::mutex engine_mutex_;
    std::thread* engine_thread_;
    eStatus engine_status_;
};

#endif // !HEAPTIMERQUEUE_H