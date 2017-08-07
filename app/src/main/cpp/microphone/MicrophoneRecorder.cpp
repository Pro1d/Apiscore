//
// Created by pgg on 02/08/17.
//


#include "MicrophoneRecorder.h"
#include <android/log.h>
#include <vector>

#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, __VA_ARGS__)
#define LOGF(...) __android_log_print(ANDROID_LOG_FATAL, __VA_ARGS__)

#define CONV16BIT 32768
#define CONVMYFLT (1./32768.)


// creates the OpenSL ES audio engine
SLresult OpenSLInputStream::openSLCreateEngine()
{
    SLresult result;
    // create engine
    result = slCreateEngine(&engineObject, 0, nullptr, 0, nullptr, nullptr);
    if(result != SL_RESULT_SUCCESS) {
        error = __LINE__;
        goto  engine_end;
    }

    // realize the engine
    result = (*engineObject)->Realize(engineObject, SL_BOOLEAN_FALSE);
    if(result != SL_RESULT_SUCCESS) {
        error = __LINE__;
        goto engine_end;
    }

    // get the engine interface, which is needed in order to create other objects
    result = (*engineObject)->GetInterface(engineObject, SL_IID_ENGINE, &(engineEngine));
    if(result != SL_RESULT_SUCCESS) {
        error = __LINE__;
        goto  engine_end;
    }

    engine_end:
    return result;
}

// Open the OpenSL ES device for input
SLresult OpenSLInputStream::openSLRecOpen(){
    SLresult result;

    switch(sr) {
        case 8000:
            sr = SL_SAMPLINGRATE_8;
            break;
        case 11025:
            sr = SL_SAMPLINGRATE_11_025;
            break;
        case 16000:
            sr = SL_SAMPLINGRATE_16;
            break;
        case 22050:
            sr = SL_SAMPLINGRATE_22_05;
            break;
        case 24000:
            sr = SL_SAMPLINGRATE_24;
            break;
        case 32000:
            sr = SL_SAMPLINGRATE_32;
            break;
        case 44100:
            sr = SL_SAMPLINGRATE_44_1;
            break;
        case 48000:
            sr = SL_SAMPLINGRATE_48;
            break;
        case 64000:
            sr = SL_SAMPLINGRATE_64;
            break;
        case 88200:
            sr = SL_SAMPLINGRATE_88_2;
            break;
        case 96000:
            sr = SL_SAMPLINGRATE_96;
            break;
        case 192000:
            sr = SL_SAMPLINGRATE_192;
            break;
        default:
            return -1;
    }

    // configure audio source
    SLDataLocator_IODevice loc_dev = {SL_DATALOCATOR_IODEVICE, SL_IODEVICE_AUDIOINPUT,
                                      SL_DEFAULTDEVICEID_AUDIOINPUT, nullptr};
    SLDataSource audioSrc = {&loc_dev, nullptr};

    // configure audio sink
    SLDataLocator_AndroidSimpleBufferQueue loc_bq = {SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE, 2};
    SLDataFormat_PCM format_pcm = {SL_DATAFORMAT_PCM, 1, sr,
                                   SL_PCMSAMPLEFORMAT_FIXED_16, SL_PCMSAMPLEFORMAT_FIXED_16,
                                   SL_SPEAKER_FRONT_CENTER, SL_BYTEORDER_LITTLEENDIAN};
    SLDataSink audioSnk = {&loc_bq, &format_pcm};

    // create audio recorder
    // (requires the RECORD_AUDIO permission)
    const SLInterfaceID id[1] = {SL_IID_ANDROIDSIMPLEBUFFERQUEUE};
    const SLboolean req[1] = {SL_BOOLEAN_TRUE};
    result = (*engineEngine)->CreateAudioRecorder(engineEngine, &(recorderObject), &audioSrc,
                                                     &audioSnk, 1, id, req);
    if (SL_RESULT_SUCCESS != result) {
        error = __LINE__;
        goto end_recopen;
    }

    // realize the audio recorder
    result = (*recorderObject)->Realize(recorderObject, SL_BOOLEAN_FALSE);
    if (SL_RESULT_SUCCESS != result) {
        error = __LINE__;
        goto end_recopen;
    }

    // get the record interface
    result = (*recorderObject)->GetInterface(recorderObject, SL_IID_RECORD, &(recorderRecord));
    if (SL_RESULT_SUCCESS != result) {
        error = __LINE__;
        goto end_recopen;
    }

    // get the buffer queue interface
    result = (*recorderObject)->GetInterface(recorderObject, SL_IID_ANDROIDSIMPLEBUFFERQUEUE,
                                                &(recorderBufferQueue));
    if (SL_RESULT_SUCCESS != result) {
        error = __LINE__;
        goto end_recopen;
    }

    // register callback on the buffer queue
    result = (*recorderBufferQueue)->RegisterCallback(recorderBufferQueue, bqRecorderCallback,
                                                         this);
    if (SL_RESULT_SUCCESS != result) {
        error = __LINE__;
        goto end_recopen;
    }
    result = (*recorderRecord)->SetRecordState(recorderRecord, SL_RECORDSTATE_RECORDING);

    end_recopen:
    return result;
}

// close the OpenSL IO and destroy the audio engine
void OpenSLInputStream::openSLDestroyEngine()
{
    // destroy audio recorder object, and invalidate all associated interfaces
    if (recorderObject != nullptr) {
        (*recorderObject)->Destroy(recorderObject);
        recorderObject = nullptr;
        recorderRecord = nullptr;
        recorderBufferQueue = nullptr;
    }

    // destroy engine object, and invalidate all associated interfaces
    if (engineObject != nullptr) {
        (*engineObject)->Destroy(engineObject);
        engineObject = nullptr;
        engineEngine = nullptr;
    }
}


// open the android audio device for input
OpenSLInputStream::OpenSLInputStream(unsigned int samplingRate, unsigned int bufferFrames) :
    error(0),
    inputBuffer(new short[bufferFrames]),
    bufferSamples(bufferFrames),
    time(0),
    sr(samplingRate)
{
    if(inputBuffer == nullptr) {
        LOGE("OpenSLInputStream", "Memory allocation failed");
        error = __LINE__;
        return;
    }

    if(openSLCreateEngine() != SL_RESULT_SUCCESS) {
        LOGE("OpenSLInputStream", "openSLCreateEngine failed");
        //error = true;
        return;
    }

    if(openSLRecOpen() != SL_RESULT_SUCCESS) {
        LOGE("OpenSLInputStream", "openSLRecOpen failed");
        //error = true;
        return;
    }

    inlock.notify();
}

// close the android audio device
OpenSLInputStream::~OpenSLInputStream()
{
    openSLDestroyEngine();

    inlock.notify();
}

// returns timestamp of the processed stream
double OpenSLInputStream::getTimestamp()
{
    return time;
}


// this callback handler is called every time a buffer finishes recording
void OpenSLInputStream::bqRecorderCallback(SLAndroidSimpleBufferQueueItf bq, void *context)
{
    OpenSLInputStream *that = (OpenSLInputStream*) context;
    that->inlock.notify();
}

// gets a buffer of size samples from the device
int OpenSLInputStream::readNextBuffer(std::vector<float> &bufferOut)
{
    short* in = inputBuffer.get();
    int size = bufferSamples;
    bufferOut.resize(size);

    inlock.wait();
    (*recorderBufferQueue)->Enqueue(recorderBufferQueue,
                                    in, size*sizeof(short));
    for(int i = 0; i < size; i++)
        bufferOut[i] = (float) (in[i] * CONVMYFLT * 2. - 1.);

    time += (double) size / sr;
    return size;
}

int OpenSLInputStream::getError()
{
    return error;
}

//----------------------------------------------------------------------
// thread Locks
// to ensure synchronisation between callbacks and processing code
ThreadLock::ThreadLock() : s(true)
{
    if (pthread_mutex_init(&m, (pthread_mutexattr_t*) nullptr) != 0) {
        LOGE("ThreadLock", "pthread_mutex_init failed");
        return;
    }
    if (pthread_cond_init(&c, (pthread_condattr_t*) nullptr) != 0) {
        LOGE("ThreadLock", "pthread_cond_init failed");
        return;
    }
}

void ThreadLock::wait()
{
    pthread_mutex_lock(&m);
    while (!s) {
        pthread_cond_wait(&c, &m);
    }
    s = false;
    pthread_mutex_unlock(&m);
}

void ThreadLock::notify()
{
    pthread_mutex_lock(&m);
    s = true;
    pthread_cond_signal(&c);
    pthread_mutex_unlock(&m);
}

ThreadLock::~ThreadLock()
{
    notify();
    pthread_cond_destroy(&c);
    pthread_mutex_destroy(&m);
}
