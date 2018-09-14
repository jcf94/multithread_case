/* ***********************************************
MYID    : Chen Fan
LANG    : G++
PROG    : MAIN
************************************************ */

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <algorithm>

#include <chrono>

#include "ThreadPool/ThreadPool.h"
#include "TimerQueue/HeapTimerQueue.h"
#include "TimerQueue/WheelTimerQueue.h"
#include "ReadWriteLock/ReadWriteLock.h"

using namespace std;

int main()
{
    ThreadPool pool(4);

    for (int i=0;i<10;i++)
    pool.add_task([i]()
    {
        printf("Hello World: %d\n", i);
        //this_thread::sleep_for(chrono::seconds(2));
    });

    // pool.wait();

    // ------------------------------

    // HeapTimerQueue timequeue(pool);
    // WheelTimerQueue timequeue(pool);

    // timequeue.start_Engine();

    // timequeue.add_Timer(2000, [](){printf("TimerQueue Hello World 1\n");});
    // timequeue.add_Timer(5000, [](){printf("TimerQueue Hello World 2\n");});
    // timequeue.add_Timer(3000, [](){printf("TimerQueue Hello World 3\n");});

    // timequeue.stop_Engine();

    // ------------------------------

    ReadWriteLock rwlock;
    int target = 0;

    thread* th[4];

    for (int i=0;i<3;i++)
    th[i] = new thread([&rwlock, &target, i]()
    {
        for (;;)
        {
            rwlock.getRead();
            printf("thread %d: %d\n", i, target);
            rwlock.unlockRead();
            this_thread::sleep_for(chrono::seconds(i+1));
        }
    });
    th[3] = new thread([&rwlock, &target]()
    {
        for (;;)
        {
            rwlock.getWrite();
            target++;
            rwlock.unlockWrite();
            this_thread::sleep_for(chrono::seconds(1));
        }
    });

    for (int i=0;i<4;i++) th[i]->join();
    
    return 0;
}