//
// Created by pgg on 02/08/17.
//

#ifndef APISCORE_MICROPHONERECORDER_H
#define APISCORE_MICROPHONERECORDER_H

#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
#include <pthread.h>
#include <cstdlib>
#include <vector>
#include <memory>

class ThreadLock {
public:
    ThreadLock();
    ~ThreadLock();
    void wait();
    void notify();
private:
    pthread_mutex_t m;
    pthread_cond_t c;
    bool s;
};

class OpenSLInputStream {
public:

    /*
    Open the audio device with a given sampling rate and IO buffer size
    in frames.
    */
    OpenSLInputStream(unsigned int samplingRate, unsigned int bufferFrames);

    /*
    Close the audio device
    */
    ~OpenSLInputStream();

    /*
    Read a buffer from the OpenSL stream *p, of size samples. Returns the number of samples read.
    */
    int readNextBuffer(std::vector<float> &buffer);

    /*
    Get the current IO block time in seconds
    */
    double getTimestamp();

    int getError();

protected:
    SLresult openSLCreateEngine();
    SLresult openSLRecOpen();
    void openSLDestroyEngine();
    static void bqRecorderCallback(SLAndroidSimpleBufferQueueItf bq, void *context);

private:
    int error;

    // engine interfaces
    SLObjectItf engineObject;
    SLEngineItf engineEngine;

    // recorder interfaces
    SLObjectItf recorderObject;
    SLRecordItf recorderRecord;
    SLAndroidSimpleBufferQueueItf recorderBufferQueue;

    // buffers
    std::unique_ptr<short> inputBuffer;

    // size of buffers
    unsigned int bufferSamples;

    // locks
    ThreadLock inlock;

    double time;

    // sampling rate
    unsigned int sr;
};

#endif //APISCORE_MICROPHONERECORDER_H
