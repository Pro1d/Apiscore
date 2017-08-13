package com.proid.apiscore;

/**
 * Created by pgg on 02/08/17.
 */

public class MicrophoneRecorder {

    static {
        System.loadLibrary("native-lib");
    }

    private static Thread thread;

    public static void startRecording() {
        thread = new Thread(new Runnable() {
            @Override
            public void run() {
                startProcess();
            }
        });
        thread.start();
    }

    public static void stopRecording() {
        stopProcess();
    }

    private static native void startProcess();
    private static native void stopProcess();

    public static native String[] getSpectrumName();
    public static native float[] getSpectrumFrequency();
    public static native float[] getSpectrumIntensity();
    public static native String[] getScore();
    /*
    static native void create();
    static native void destroy();
    static native void start();
    static native void stop();
    static native void read();
    static native boolean isRecording();
    */
}
