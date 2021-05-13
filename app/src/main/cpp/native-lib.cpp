#include <jni.h>
#include <string>
#include "MyPlayer.h"
#include "JNICallback.h"
#include <android/native_window_jni.h> // 是为了 渲染到屏幕支持的
extern "C" {
#include <libavutil/avutil.h>
}

JavaVM *javaVm = 0;
MyPlayer *myPlayer = 0;
ANativeWindow * nativeWindow = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // 静态初始化 互斥锁

int JNI_OnLoad(JavaVM *javaVm, void *pVoid) {
    ::javaVm = javaVm;
    return JNI_VERSION_1_6;//注意，这里必须返回，如同线程要返回一样，否则报错
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_hqk_play_1ndk_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "FFmpeg 当前版本号：";
    hello.append(av_version_info());
    return env->NewStringUTF(hello.c_str());
}



/**
 * 专门渲染的函数
 * @param src_data 解码后的视频 rgba 数据
 * @param width 宽信息
 * @param height 高信息
 * @param src_liinesize 行数size相关信息
 */
void renderFrame(uint8_t * src_data, int width, int height, int src_liinesize) {
    pthread_mutex_lock(&mutex);

    if (!nativeWindow) {
        pthread_mutex_unlock(&mutex);
    }

    // 设置窗口属性
    ANativeWindow_setBuffersGeometry(nativeWindow, width, height , WINDOW_FORMAT_RGBA_8888);

    ANativeWindow_Buffer windowBuffer;
    if (ANativeWindow_lock(nativeWindow, &windowBuffer, 0)) {
        ANativeWindow_release(nativeWindow);
        nativeWindow = 0;
        pthread_mutex_unlock(&mutex);
        return;
    }

    // 填数据到buffer，其实就是修改数据
    uint8_t * dst_data = static_cast<uint8_t *>(windowBuffer.bits);
    int lineSize = windowBuffer.stride * 4; // RGBA
    // 下面就是逐行Copy了
    for (int i = 0; i < windowBuffer.height; ++i) {
        // 一行Copy
        memcpy(dst_data + i * lineSize, src_data + i * src_liinesize, lineSize);
    }

    ANativeWindow_unlockAndPost(nativeWindow);
    pthread_mutex_unlock(&mutex);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_hqk_play_1ndk_MyPlayer_prepareNative(JNIEnv *env, jobject thiz, jstring data_source) {

    JNICallback *jniCallback = new JNICallback(javaVm, env, thiz);

    const char *dataSource = env->GetStringUTFChars(data_source, NULL);
    //准备功能，面向对象 方式，创建MyPlayer
    myPlayer = new MyPlayer(dataSource,jniCallback);
    myPlayer->setRenderCallback(renderFrame);
    myPlayer->prepare();

    env->ReleaseStringUTFChars(data_source, dataSource);

}

extern "C"
JNIEXPORT void JNICALL
Java_com_hqk_play_1ndk_MyPlayer_startNative(JNIEnv *env, jobject thiz) {
    // TODO: implement startNative()
    if (myPlayer){
        myPlayer->start();
    }
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