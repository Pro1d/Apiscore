#include <jni.h>
#include <string>
#include <algorithm>
#include "microphone/MicrophoneRecorder.h"
#include "FrequencyAnalyzer.h"
#include "spectrum/filters/MeanNoiseRemoval.h"
#include "spectrum/filters/NonMaxSuppression.h"
#include "PointerSwitcher.h"
#include <android/log.h>
#include "Music/NotePitch.h"
#include "spectrum/SpectrumHistory.h"
#include "CircularBuffer.h"

#define STR_(x) #x
#define STR(x) STR_(x)
#define ERROR_JNI_CALL(CONST_MSG) __android_log_print(ANDROID_LOG_ERROR, "JNICALL", __FILE__ ":" STR(__LINE__) " - " CONST_MSG)

extern "C"
JNIEXPORT jstring JNICALL
Java_com_proid_apiscore_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

extern "C"
JNIEXPORT void JNICALL
Java_com_proid_apiscore_MainActivity_fillBytes(JNIEnv *env, jobject, jbyteArray bytes) {
    jbyte* tempPointer = env->GetByteArrayElements(bytes, 0);
    //const char* cinput = (const char*) tempPointer;
    int dataSize = (int)env->GetArrayLength(bytes);
    for(int i = dataSize; --i >= 0;)
        tempPointer[i] = (jbyte) 42;
    env->ReleaseByteArrayElements(bytes, tempPointer, 0);
}

#define SR                      44100 // Hertz
#define PROCESS_PERIOD          20 // milliseconds
#define OVERLAPPING_PERIODS     2
#define BUFFER_PERIOD           (PROCESS_PERIOD / OVERLAPPING_PERIODS)
#define BUFFER_SAMPLES          (SR * BUFFER_PERIOD / 1000)
#define PROCESS_SAMPLES         (SR * PROCESS_PERIOD / 1000)
#define HISTORY_TIME            1000 // milliseconds
#define HISTORY_SIZE            (HISTORY_TIME * OVERLAPPING_PERIODS / PROCESS_PERIOD)

static bool on;
static SpectrumDefinition::Ptr spectrum;
static SampledSpectrumDefinition::Ptr sampledSpectrum;
static PointerSwitcher<Spectrum> intensityPtrs;
static SpectrumHistory::Ptr spectrumHistory;

extern "C"
JNIEXPORT void JNICALL
Java_com_proid_apiscore_MicrophoneRecorder_startProcess()
{
    spectrum.reset(new SpectrumDefinition(3*SEMITONES_PER_SCALE, 5*SEMITONES_PER_SCALE));
    sampledSpectrum.reset(new  SampledSpectrumDefinition(*spectrum, PROCESS_SAMPLES, SR));
    intensityPtrs.reset(new Spectrum(spectrum->size), new Spectrum(spectrum->size));
    spectrumHistory.reset(new SpectrumHistory(HISTORY_SIZE));
    CircularBuffer<float> cbuffer(PROCESS_SAMPLES);

    OpenSLInputStream inputStream(SR, BUFFER_SAMPLES);
    FrequencyAnalyzer fa(SR);

    std::vector<float> inbuffer(BUFFER_SAMPLES);
    on = true;
    while(on && !inputStream.getError()) {
        intensityPtrs.lockForScope();
        Spectrum::Ptr& intensity = intensityPtrs.getWrite();

        double timeBegin = inputStream.getTimestamp();
        inputStream.readNextBuffer(inbuffer);
        double timeEnd = inputStream.getTimestamp();

        cbuffer.push(inbuffer);

        intensity->setAcquisitionStart(timeBegin);
        intensity->setAcquisitionEnd(timeEnd);
        fa.getSpectrumIntensity(cbuffer, PROCESS_SAMPLES, *sampledSpectrum, intensity);

        intensity->printStat();

        Spectrum::Ptr tmp(new Spectrum);

        //*tmp = *intensity;
        /*
        MeanNoiseRemoval mnr(3.);
        mnr.setInput(intensity);
        mnr.filter(*tmp);//*/
        //*intensity = *tmp;
        /*
        NonMaxSuppression nms;
        nms.setInput(tmp);
        nms.filter(*intensity);//*/


        //__android_log_print(ANDROID_LOG_VERBOSE, "###", "Read %d samples", samps);
        intensity->printStat();

        spectrumHistory->addSpectrum(*intensity);

        intensityPtrs.unsafeSwitchReadWrite();
    }
    __android_log_print(ANDROID_LOG_DEBUG, "###", "Exit with error %d", inputStream.getError());
}

extern "C"
JNIEXPORT void JNICALL
Java_com_proid_apiscore_MicrophoneRecorder_stopProcess()
{
    on = false;
}

template <typename T>
jfloatArray vectorToJFloatArray(JNIEnv *env, std::vector<T> const& vec) {
    int size = vec.size();
    jfloat *data = new jfloat[size];
    for(int i = 0; i < size; i++)
        data[i] = vec[i];

    jfloatArray array = env->NewFloatArray(size);
    env->SetFloatArrayRegion(array, 0, size, data);
    delete[] data;
    return array;
}

extern "C"
JNIEXPORT jfloatArray JNICALL
Java_com_proid_apiscore_MicrophoneRecorder_getSpectrumFrequency(JNIEnv *env)
{
    if(spectrum == nullptr) {
        ERROR_JNI_CALL("Must call startProcess first");
        return env->NewFloatArray(0);
    }

    return vectorToJFloatArray<float>(env, spectrum->frequency);
}

extern "C"
JNIEXPORT jfloatArray JNICALL
Java_com_proid_apiscore_MicrophoneRecorder_getSpectrumIntensity(JNIEnv *env)
{
    intensityPtrs.lockForScope();

    if(intensityPtrs.getRead() == nullptr) {
        ERROR_JNI_CALL("Null ptr ");
        return env->NewFloatArray(0);
    }

    return vectorToJFloatArray<float>(env, *intensityPtrs.getRead());
}

extern "C"
JNIEXPORT jobjectArray JNICALL
Java_com_proid_apiscore_MicrophoneRecorder_getSpectrumName(JNIEnv *env)
{
    if(spectrum == nullptr) {
        ERROR_JNI_CALL("Must call startProcess first");
        return env->NewObjectArray(0, env->FindClass("java/lang/String"), env->NewStringUTF(""));
    }

    int size = spectrum->name.size();
    jobjectArray result = (jobjectArray) env->NewObjectArray(size, env->FindClass("java/lang/String"), env->NewStringUTF(""));

    for(int i = 0; i < size; i++)
        env->SetObjectArrayElement(result, i, env->NewStringUTF(spectrum->name[i].c_str()));

    return result;
}

/*
static MicrophoneRecorder* mr = nullptr;

extern "C"
JNIEXPORT void JNICALL
Java_com_proid_apiscore_MicrophoneRecorder_create() {
    if (mr == nullptr) {
        mr = new MicrophoneRecorder();
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_proid_apiscore_MicrophoneRecorder_destroy()
{
    if(mr == nullptr) {
        ERROR_JNI_CALL("Call create() before");
        return;
    }
    delete mr;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_proid_apiscore_MicrophoneRecorder_start()
{
    if(mr == nullptr) {
        ERROR_JNI_CALL("Call create() before");
        return;
    }
    mr->startRecording();
}


extern "C"
JNIEXPORT void JNICALL
Java_com_proid_apiscore_MicrophoneRecorder_stop()
{
    if(mr == nullptr) {
        ERROR_JNI_CALL("Call create() before");
        return;
    }
    mr->stopRecording();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_proid_apiscore_MicrophoneRecorder_read()
{
    if(mr == nullptr) {
        ERROR_JNI_CALL("Call create() before");
        return;
    }
    mr->readNextFrames();
}


extern "C"
JNIEXPORT bool JNICALL
Java_com_proid_apiscore_MicrophoneRecorder_isRecording()
{
    if(mr == nullptr) {
        ERROR_JNI_CALL("Call create() before");
        return false;
    }
    return mr->isRecording();
}

*/