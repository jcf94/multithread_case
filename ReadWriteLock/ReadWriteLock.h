/* ***********************************************
MYID   : Chen Fan
LANG   : G++
PROG   : READWRITELOCK_H
************************************************ */

#ifndef READWRITELOCK_H
#define READWRITELOCK_H

#include <mutex>
#include <condition_variable>

class ReadWriteLock
{
public:
    ReadWriteLock();
    ~ReadWriteLock();

    void getRead();
    void getWrite();
    void unlockRead();
    void unlockWrite();

private:
    int read_count_;
    bool is_writing_;
    std::mutex cv_mutex_;
    std::condition_variable cv_;
};

#endif // !READWRITELOCK_H