#pragma once

#include "messagequeue.h"
#include <string>

/**
* Main class - enumerates files and sends them to the message queue
*/
class FileEnumerator
{
public:
    FileEnumerator();
    virtual ~FileEnumerator();

    /**
    * Class run method that does all the work managing the queue and
    * creating work for the encoders
    * @param searchDir Path to the directory that is to be searched for WAV files
    */
    void run(const std::string& searchDir);

private:
    void enumerateFiles(const std::string& searchDir);
#ifndef WIN32
    bool caseInsensitiveStringCompare(const std::string& str1, const std::string& str2);
#endif

    MessageQueue m_messageQueue;
};
