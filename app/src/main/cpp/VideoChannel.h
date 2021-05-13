//
// Created by MC on 2021/5/12.
//

#ifndef PLAY_NDK_VIDEOCHANNEL_H
#define PLAY_NDK_VIDEOCHANNEL_H

#include "BaseChannel.h"
typedef void (*RenderCallback) (uint8_t * , int , int, int);

class VideoChannel : public BaseChannel {

public:
    VideoChannel(int stream_index, AVCodecContext *avCodecContext);

    virtual ~VideoChannel();

    void start();

    void stop();

    pthread_t pid_video_decode;
    pthread_t pid_video_play;

    void video_decode_();

    void video_play_();
};


#endif //PLAY_NDK_VIDEOCHANNEL_H
