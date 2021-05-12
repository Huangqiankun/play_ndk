//
// Created by MC on 2021/5/12.
//

#ifndef PLAY_NDK_MYPLAYER_H
#define PLAY_NDK_MYPLAYER_H

#include <cstring>
#include <pthread.h>
#include <libavformat/avformat.h>

class MyPlayer {
public:
    MyPlayer();

    MyPlayer(const char *string);

    virtual ~MyPlayer();

    void prepare();

    void prepare_();

private:
      char *data_source = 0;
      pthread_t pid_prepare=0;
      AVFormatContext *avFormatContext=0; //媒体的总上下文

};


#endif //PLAY_NDK_MYPLAYER_H
