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

#include "ThreadPool.h"

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

    pool.wait();
    
    return 0;
}