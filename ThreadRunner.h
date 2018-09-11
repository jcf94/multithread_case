/* ***********************************************
MYID   : Chen Fan
LANG   : G++
PROG   : THREADRUNNER_H
************************************************ */

#ifndef THREADRUNNER_H
#define THREADRUNNER_H

#include "ThreadPool.h"

class ThreadRunner
{
public:
    ThreadRunner(ThreadPool &s): pool_(s) {}
    void operator()();

private:
    ThreadPool &pool_;
};

#endif // !THREADRUNNER_H