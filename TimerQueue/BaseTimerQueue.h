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

namespace Clock
{

static int64_t get_CurrentTime()
{
    auto now = std::chrono::system_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
}

}

struct Timer
{
    int64_t time = 0;
    std::function<void()> func;
};

class BaseTimerQueue
{
public:
    BaseTimerQueue(){}
    virtual ~BaseTimerQueue(){}

    void add_Timer(int wait_time, std::function<void()> func);

protected:
};

#endif // !BASE_TIMERQUEUE_H