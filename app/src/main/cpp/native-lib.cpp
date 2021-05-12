#include <jni.h>
#include <string>
#include "MyPlayer.h"

extern "C" {
#include <libavutil/avutil.h>
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_hqk_play_1ndk_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "FFmpeg 当前版本号：";
    hello.append(av_version_info());
    return env->NewStringUTF(hello.c_str());
}
extern "C"
JNIEXPORT void JNICALL
Java_com_hqk_play_1ndk_MyPlayer_prepareNative(JNIEnv *env, jobject thiz, jstring data_source) {

    const char *dataSource = env->GetStringUTFChars(data_source, NULL);
    //准备功能，面向对象 方式，创建MyPlayer
    MyPlayer *myPlayer = new MyPlayer(dataSource);
    myPlayer->prepare();

    env->ReleaseStringUTFChars(data_source, dataSource);

}

extern "C"
JNIEXPORT void JNICALL
Java_com_hqk_play_1ndk_MyPlayer_startNative(JNIEnv *env, jobject thiz) {
    // TODO: implement startNative()
}

extern "C"
JNIEXPORT void JNICALL
Java_com_hqk_play_1ndk_MyPlayer_stopNative(JNIEnv *env, jobject thiz) {
    // TODO: implement stopNative()
}

extern "C"
JNIEXPORT void JNICALL
Java_com_hqk_play_1ndk_MyPlayer_releaseNative(JNIEnv *env, jobject thiz) {
    // TODO: implement releaseNative()
}

extern "C"
JNIEXPORT void JNICALL
Java_com_hqk_play_1ndk_MyPlayer_setSurfaceNative(JNIEnv *env, jobject thiz, jobject surface) {
    // TODO: implement setSurfaceNative()
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_hqk_play_1ndk_MyPlayer_getFFmpegVersion(JNIEnv *env, jobject thiz) {
    // TODO: implement getFFmpegVersion()
}