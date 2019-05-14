#pragma once

#include <string>
#include <cstdio>

/**
* Lame library access, contains method to convert WAV to MP3
*/
class LameEncoder
{
public:
    LameEncoder();
    virtual ~LameEncoder();

    /**
    * Encode MP3 file into the destination from the source WAV file.
    * @param source Path to the input WAV file
    * @param destination Path to the output MP3 file
    */
    void encodeMp3(const std::string& source, const std::string& destination);

private:
    FILE* m_fileIn;
    FILE* m_fileOut;
};