#include "fileenumerator.h"
#ifdef WIN32
    #include <Windows.h>
#else
    #include <dirent.h>
#endif

FileEnumerator::FileEnumerator()
{
}

FileEnumerator::~FileEnumerator()
{
}

void FileEnumerator::run(const std::string& searchDir)
{
    m_messageQueue.start();

    enumerateFiles(searchDir);

    m_messageQueue.stop();
}

void FileEnumerator::enumerateFiles(const std::string& searchDir)
{
#ifdef WIN32
    std::string searchPattern(searchDir + "\\*.wav");

    WIN32_FIND_DATA ffd;
    HANDLE findRes = FindFirstFile(searchPattern.c_str(), &ffd);

    if (INVALID_HANDLE_VALUE == findRes)
        return;

    do
    {
        if (!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
        {
            std::string filePath = searchDir + "\\" + ffd.cFileName;
            // Add message to the queue
            m_messageQueue.sendMessage(filePath);
        }
    } while (FindNextFile(findRes, &ffd) != 0);

    FindClose(findRes);
#else
    DIR* d;
    struct dirent* dir;
    d = opendir(searchDir.c_str());
    if (!d)
        return;

    while ((dir = readdir(d)) != NULL)
    {
        if (DT_REG == dir->d_type)
        {
            std::string fileName(dir->d_name);
            if (compare(fileName.substr(fileName.length() - 3) "wav"))
                m_messageQueue.sendMessage(fileName);
        }
    }

    closedir(d);
#endif
}

#ifndef WIN32
bool FileEnumerator::caseInsensitiveStringCompare(const string& str1, const string& str2)
{
    if (str1.size() != str2.size())
        return false;

    for (string::const_iterator c1 = str1.begin(), c2 = str2.begin(); c1 != str1.end(); ++c1, ++c2)
    {
        if (tolower(*c1) != tolower(*c2))
            return false;
    }
    return true;
}
#endif
