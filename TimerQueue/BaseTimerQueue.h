/* ***********************************************
MYID   : Chen Fan
LANG   : G++
PROG   : BASE_TIMERQUEUE_H
************************************************ */

#ifndef BASE_TIMERQUEUE_H
#define BASE_TIMERQUEUE_H

#include <stdint.h>
#include <functional>
#include <chrono>

static int64_t get_CurrentTime()
{
    auto now = std::chrono::system_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
}

enum eStatus
{
    IDLE, WORKING, ENDING
};

struct Timer
{
    int64_t time = 0;
    std::function<void()> func;
};

class BaseTimerQueue
{
public:
    BaseTimerQueue(){}
    virtual ~BaseTimerQueue() {}

    virtual void start_Engine(int interval_ms = 50) = 0;
    virtual void stop_Engine(bool force = false) = 0;

    virtual void add_Timer(int wait_time_ms, std::function<void()> func) = 0;

protected:
};

#endif // !BASE_TIMERQUEUE_H