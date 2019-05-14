#pragma once

#include "pthreads/pthread.h"

/**
* RAII based mutex locker/unlocker
*/
class MutexLocker
{
public:
    /**
    * Constructor
    * @param mutex Mutex to lock
    */
    MutexLocker(pthread_mutex_t& mutex);

    /**
    * Destructor - the mutex locked in the constructor is
    * unlocker, if still in lock state
    */
    virtual ~MutexLocker();

    /**
    * Unlock the mutex passed on at construction time
    */
    void unlock();

private:
    pthread_mutex_t& m_mutex;
};
