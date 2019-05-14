#pragma once

#include "pthreads/pthread.h"
#include <string>
#include <queue>
#include <list>

/**
* Threaded class to read and write from the queue
*/
class MessageQueue
{
public:
    MessageQueue();
    virtual ~MessageQueue();

    /**
    * A function to send a message / place it on the queue for processing
    * @param message The message that should be sent
    */
    void sendMessage(const std::string& message);

    /**
    * Processes any outstanding events
    * @param prc Pointer to this
    * @return always NULL
    */
    static void* process(void* prc);

    /**
    * Starts the thread for processing events
    */
    void start();

    /**
    * Stops the queue, waits for all threads to finish work before returning control
    */
    void stop();

private:
    int numberOfProcessor();
    void createThreads();
    void waitForThreads();
    void processMessage(const std::string& message);

    std::list<pthread_t> m_threads;
    pthread_mutex_t m_mutex;
    pthread_cond_t m_cond;
    std::queue<std::string> m_queue;

    bool m_running;
};
