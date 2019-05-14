#pragma once

#include <exception>
#include <string>

/**
* A custom encoder exception
*/
class EncoderException : public std::exception
{
public:
    /**
    * Exception constuctor
    * @param message The error message
    * @param file The file name where the exception originated
    * @param line File line number where the exception originated
    */
    EncoderException(const char* message, const char* file, unsigned int line)
        : m_message(message), m_file(file), m_lineNumber(line)
    {
    }

    /**
    * Get exception description
    * @return Exception description
    */
    const char* message()
    {
        return m_message.c_str();
    }

private:
    std::string m_message;
    std::string m_file;
    unsigned int m_lineNumber;
};
