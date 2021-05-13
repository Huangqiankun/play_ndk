//
// Created by MC on 2021/5/12.
//

#ifndef PLAY_NDK_MYPLAYER_H
#define PLAY_NDK_MYPLAYER_H

#include <cstring>
#include <pthread.h>
#include "VideoChannel.h"
#include "AudioChannel.h"
#include "JNICallback.h"

extern "C" {

#include <libavformat/avformat.h>
};

//定义TAG之后，我们可以在LogCat通过TAG过滤出NDK打印的日志
#define TAG "MyPlayer"
// 定义info信息
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,TAG,__VA_ARGS__)
// 定义debug信息
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, TAG, __VA_ARGS__)
// 定义error信息
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,TAG,__VA_ARGS__)


class MyPlayer {
public:
    MyPlayer();

//    MyPlayer(const char *string);

    MyPlayer(const char *string, JNICallback *pCallback);

    virtual ~MyPlayer();

    void prepare();

    void prepare_();

    void start();

    void start_();

    void setRenderCallback(void (*fun)(uint8_t *, int, int, int));

private:
    char *data_source = 0;
    pthread_t pid_prepare = 0;
    AVFormatContext *avFormatContext = 0; //媒体的总上下文
    JNICallback *jniCallback = 0;

    AudioChannel *audioChannel = 0;
    VideoChannel *videoChannel = 0;


    int isPlayer = 0;
    pthread_t pid_start = 0;
    RenderCallback renderCallback;
};


#endif //PLAY_NDK_MYPLAYER_H
