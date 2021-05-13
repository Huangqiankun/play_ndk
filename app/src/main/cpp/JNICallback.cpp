//
// Created by MC on 2021/5/12.
//

#include "JNICallback.h"

JNICallback::~JNICallback() {
    this->javaVm = 0;
    env->DeleteGlobalRef(this->instance);
    this->instance = 0;
    env = 0;
}

JNICallback::JNICallback(JavaVM *javaVm, JNIEnv *env, jobject instance) {
    this->javaVm = javaVm;
    this->env = env;
    // 什么情况下，不能使用  JNIEnv *env，跨线程的时候


    //jobject instance 有可能被对方那边给释放掉
//    this->instance = instance;
    this->instance = env->NewGlobalRef(instance);//全局引用

    //回调给上层的函数
    jclass myPlayerClass = env->GetObjectClass(this->instance);
    const char *sig = "()V";
    this->jmd_repared = env->GetMethodID(myPlayerClass, "onPrepared", sig);

    sig = "(I)V";
    this->jmd_error = env->GetMethodID(myPlayerClass, "onError", sig);

}

//可能是主线程 或者异步线程
void JNICallback::onPrepared(int thread_mode) {
    LOGE("onPrepared");
    if (thread_mode == THREAD_MAIN) {
        //主线程，env 可以用
        this->env->CallVoidMethod(this->instance, this->jmd_repared);//调用上层
    } else if (thread_mode == THREAD_CHILD) {
        //子线程，不能用env
        //开始附加线程，JavaVM附加
        JNIEnv *jniEnv = nullptr;
        int ret = this->javaVm->AttachCurrentThread(&jniEnv, 0);
        if (ret != JNI_OK) {
            return;
        }
        jniEnv->CallVoidMethod(this->instance, this->jmd_repared);
        //解除附加，释放工作
        this->javaVm->DetachCurrentThread();
    }
}

//可能是主线程 或者异步线程
void JNICallback::onErrorAction(int thread_mode, int error_code) {
    if (thread_mode == THREAD_MAIN) {
        //主线程，env 可以用
        this->env->CallVoidMethod(this->instance, this->jmd_error,error_code);//调用上层
    } else if (thread_mode == THREAD_CHILD) {
        //子线程，不能用env
        //开始附加线程，JavaVM附加
        JNIEnv *jniEnv = nullptr;
        LOGE("javaVm javaVm ");
        int ret = this->javaVm->AttachCurrentThread(&jniEnv, 0);
        LOGE("javaVm ret %d",ret);
        if (ret != JNI_OK) {
            return;
        }
        jniEnv->CallVoidMethod(this->instance, this->jmd_error,error_code);
        //解除附加，释放工作
        this->javaVm->DetachCurrentThread();
    }
}
