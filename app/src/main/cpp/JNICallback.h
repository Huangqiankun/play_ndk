//
// Created by MC on 2021/5/12.
//

#ifndef PLAY_NDK_JNICALLBACK_H
#define PLAY_NDK_JNICALLBACK_H

#include <jni.h>
#include <android/log.h>
#include "macro.h"

//定义TAG之后，我们可以在LogCat通过TAG过滤出NDK打印的日志
#define TAG "JNICallback"
// 定义info信息
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,TAG,__VA_ARGS__)
// 定义debug信息
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, TAG, __VA_ARGS__)
// 定义error信息
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,TAG,__VA_ARGS__)

class JNICallback {

public:
    //线程中，不能使用 env，env会失效
    JNICallback(JavaVM *javaVm, JNIEnv *env, jobject instance);

    virtual ~JNICallback();

    //具体回调给上层的函数(成功)
    void onPrepared(int thread_mode);//thread_mode 主线程/异步线程

    //具体回调给上层的函数(失败)
    void onErrorAction(int thread_mode, int error_code);

private: //指针最好默认赋值0，非指针可以不用赋值默认值
    JavaVM *javaVm = 0;
    JNIEnv *env = 0;
    jobject instance;

    jmethodID jmd_repared;
    jmethodID jmd_error;

};


#endif //PLAY_NDK_JNICALLBACK_H
