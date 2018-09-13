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
#include "TimerQueue/TimerQueue.h"

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

    //pool.wait();

    // ------------------------------

    TimerQueue timequeue(pool);

    timequeue.start_Engine();

    timequeue.add_Timer(2000, [](){printf("TimerQueue Hello World 1\n");});
    timequeue.add_Timer(5000, [](){printf("TimerQueue Hello World 2\n");});
    timequeue.add_Timer(3000, [](){printf("TimerQueue Hello World 3\n");});

    timequeue.stop_Engine();
    
    return 0;
}