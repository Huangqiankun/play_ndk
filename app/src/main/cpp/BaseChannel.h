//
// Created by MC on 2021/5/12.
//

#ifndef PLAY_NDK_BASECHANNEL_H
#define PLAY_NDK_BASECHANNEL_H


#include "safe_queue.h"
#include <cstring>
#include <pthread.h>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
};

class BaseChannel {

public:
    int stream_index;
    AVCodecContext *avCodecContext;

    BaseChannel(int stream_index, AVCodecContext *avCodecContext) {
        this->avCodecContext = avCodecContext;
        this->stream_index = stream_index;
    }

    virtual ~BaseChannel() {

    }

    //音频 aac; 视频：h264
    SafeQueue<AVPacket *> packets;

    // 音频 pcm ；视频 yuv
    SafeQueue<AVFrame *> frames;


    bool isPlaying = 1;
};


#endif //PLAY_NDK_BASECHANNEL_H
