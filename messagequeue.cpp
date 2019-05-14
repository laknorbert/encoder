#include "messagequeue.h"
#include "mutexlocker.h"
#include "lameencoder.h"
#include "encoderexception.h"
#include <iostream>
#include <thread>

MessageQueue::MessageQueue()
    : m_running(false)
{
    pthread_mutex_init(&m_mutex, NULL);
    pthread_cond_init(&m_cond, NULL);
}

MessageQueue::~MessageQueue()
{
    pthread_mutex_destroy(&m_mutex);
    pthread_cond_destroy(&m_cond);
}

void MessageQueue::sendMessage(const std::string& message)
{
    MutexLocker ml(m_mutex);

    // Add to the message backlog container
    m_queue.push(message);

    ml.unlock();

    // Notify any thread to start conversion
    pthread_cond_signal(&m_cond);
}

void* MessageQueue::process(void* threadData)
{
    MessageQueue* prc = (MessageQueue*)threadData;

    while (prc->m_running || !prc->m_queue.empty())
    {
        MutexLocker ml(prc->m_mutex);
        if (prc->m_queue.empty())
            pthread_cond_wait(&prc->m_cond, &prc->m_mutex);

        std::string message;
        if (!prc->m_queue.empty())
        {
            message = prc->m_queue.front();
            prc->m_queue.pop();
        }
        ml.unlock();

        if (!message.empty())
            prc->processMessage(message);
    }

    pthread_exit(NULL);
    return NULL; // Just to satisfy the compiler
}

void MessageQueue::processMessage(const std::string& message)
{
    std::string srcFile(message);
    std::string destFile(srcFile.substr(0, srcFile.length() - 3) + "mp3");

    try
    {
        LameEncoder encoder;
        encoder.encodeMp3(srcFile, destFile);
    }
    catch (EncoderException& ex)
    {
        // Do some logging, or reporting, or ...
        std::wcout << ex.message() << std::endl;
    }
}

void MessageQueue::start()
{
    m_running = true;

    createThreads();
}

void MessageQueue::createThreads()
{
    int numThreads = numberOfProcessor();

    for (int thread = 0; thread < numThreads; ++thread)
    {
        pthread_t tid;
        pthread_create(&tid, NULL, &MessageQueue::process, this);
        m_threads.push_back(tid);
    }
}

void MessageQueue::waitForThreads()
{
    std::list<pthread_t>::const_iterator iter;
    for (iter = m_threads.begin(); iter != m_threads.end(); ++iter)
        pthread_join(*iter, NULL);
}

void MessageQueue::stop()
{
    // Mark us as stopped
    m_running = false;
    pthread_cond_broadcast(&m_cond);

    // Wait for the threads to exit
    waitForThreads();
}

int MessageQueue::numberOfProcessor()
{
    return std::thread::hardware_concurrency();

 /*
 // For WIN, could be also
    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);

    return sysinfo.dwNumberOfProcessors;
 */
}
