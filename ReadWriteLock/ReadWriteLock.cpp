/* ***********************************************
MYID   : Chen Fan
LANG   : G++
PROG   : READWRITELOCK
************************************************ */

#include "ReadWriteLock.h"

ReadWriteLock::ReadWriteLock()
    : read_count_(0), is_writing_(false) {}

ReadWriteLock::~ReadWriteLock()
{

}

void ReadWriteLock::getRead()
{
    std::unique_lock<std::mutex> lock(cv_mutex_);
    
    cv_.wait(lock, [this](){return !this->is_writing_;});

    read_count_++;
}

void ReadWriteLock::getWrite()
{
    std::unique_lock<std::mutex> lock(cv_mutex_);

    cv_.wait(lock, [this](){return this->read_count_==0 && !this->is_writing_;});

    is_writing_ = true;
}

void ReadWriteLock::unlockRead()
{
    std::lock_guard<std::mutex> lock(cv_mutex_);
    read_count_--;
    if (read_count_ == 0)
    {
        cv_.notify_one();
    }
}

void ReadWriteLock::unlockWrite()
{
    std::lock_guard<std::mutex> lock(cv_mutex_);
    is_writing_ = false;
    cv_.notify_one();
}