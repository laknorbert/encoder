#include "mutexlocker.h"

MutexLocker::MutexLocker(pthread_mutex_t& mutex)
    : m_mutex(mutex)
{
    pthread_mutex_lock(&m_mutex);
}

MutexLocker::~MutexLocker()
{
    unlock();
}

void MutexLocker::unlock()
{
    pthread_mutex_unlock(&m_mutex);
}
