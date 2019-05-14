#include "lameencoder.h"
#include "lame/lame.h"
#include "encoderexception.h"
#include <iostream>

LameEncoder::LameEncoder()
    : m_fileIn(NULL), m_fileOut(NULL)
{
}

LameEncoder::~LameEncoder()
{
    if (NULL != m_fileIn)
        fclose(m_fileIn);

    if (NULL != m_fileOut)
        fclose(m_fileIn);
}

void LameEncoder::encodeMp3(const std::string& source, const std::string& destination)
{
    // Try to open the WAV file
    m_fileIn = fopen(source.c_str(), "rb");
    if (NULL == m_fileIn)
        throw EncoderException("Error opening source file", __FILE__, __LINE__);

    // Open MP3 file
    m_fileOut = fopen(destination.c_str(), "wb");
    if (NULL == m_fileOut)
        throw EncoderException("Error opening destination file", __FILE__, __LINE__);

    // Init lame
    lame_t lame = lame_init();

    // Do some settings, and initialize parameters
    // Default settings used for now, probably needs some tweaking or params
    lame_set_VBR(lame, vbr_default);

    int ret = lame_init_params(lame);
    if (ret < 0)
        throw EncoderException("Error initializing lame library", __FILE__, __LINE__);


    const int PCM_SIZE = 8192;
    const int MP3_SIZE = 8192;

    short pcm_buffer[PCM_SIZE * 2];
    unsigned char mp3_buffer[MP3_SIZE];

    size_t nread, nwrite;

    do
    {
        nread = fread(pcm_buffer, 2 * sizeof(short), PCM_SIZE, m_fileIn);

        if (nread != 0)
        {
            // Encode
            int nsamples = nread;

            // ToDo: online example of code, should be possible to allocate the buffer
            //       only once
            short* buffer_l = new short[nsamples];
            short* buffer_r = new short[nsamples];

            int j = 0;
            int i = 0;
            for (i = 0; i < nsamples; i++)
            {
                buffer_l[i] = pcm_buffer[j++];
                buffer_r[i] = pcm_buffer[j++];
            }

            nwrite = lame_encode_buffer(lame, buffer_l, buffer_r, nsamples, mp3_buffer, MP3_SIZE);

            delete [] buffer_l;
            delete [] buffer_r;
        }
        else
        {
            // Flush and give some final frames
            nwrite = lame_encode_flush(lame, mp3_buffer, MP3_SIZE);
        }

        if (nwrite  < 0)
            throw EncoderException("Error encoding WAV stream", __FILE__, __LINE__);

        fwrite(mp3_buffer, nwrite, 1, m_fileOut);
    } while (nread != 0);

    // Free internal data structures
    lame_close(lame);
}
