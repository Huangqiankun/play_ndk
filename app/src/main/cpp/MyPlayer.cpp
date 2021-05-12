//
// Created by MC on 2021/5/12.
//


#include "MyPlayer.h"

//函数指针
//void* (*__start_routine)(void*)
void *customTaskPrepareThread(void *pVoid) {
    MyPlayer *myPlayer = static_cast<MyPlayer *>(pVoid);
    myPlayer->prepare_();
    return 0;//必须返回，有坑！
}


MyPlayer::MyPlayer() {}

MyPlayer::~MyPlayer() {
    if (this->data_source) {
        delete this->data_source;
        this->data_source = 0;
    }
}

MyPlayer::MyPlayer(const char *data_source) {
//    // 有坑，会报错，容易出现悬空指针
//    this->data_source=data_source;

//    //这里还是会有坑，长度  不对
//    this->data_source = new char[strlen(data_source)]

    //这样才对！
    this->data_source = new char[strlen(data_source) + 1];
    stpcpy(this->data_source, data_source);
}

// 准备工作失去，“解码”，拆包裹（音视频，视频流，字幕流.....）
void MyPlayer::prepare() {
    //创建异步线程
    pthread_create(&this->pid_prepare, 0, customTaskPrepareThread, this);
}

//异步的函数
void MyPlayer::prepare_() {
    //拆包裹 --解复用，解封装，解码

    //获取生成上下文
    this->avFormatContext = avformat_alloc_context();

    //此字典 能够决定 我们打开的需求
    AVDictionary *dictionary = 0;
    av_dict_set(&dictionary, "timeout", "5000000", 0);//注意：单位是微妙;如果 ，你的模拟器很卡，那尽量设置大一点

    // 是不是包裹，如果对方寄来的是 非包裹（被损坏的数据），那就没法玩了
    int ret = avformat_open_input(&this->avFormatContext, this->data_source, 0, &dictionary);
    if (ret) {
        // ...todo..写JNI回调，告诉java 层，通知用户，你的播放流损坏
        return;
    }
    //因为下面不需要字典，所以可回收字典
    av_dict_free(&dictionary);
    //寻找，媒体格式中的，（音频，视频，字幕）
    ret = avformat_find_stream_info(this->avFormatContext, 0);//为什么第二个参数，不给字典，不需要设置格外配置
    if (ret < 0) {
        // ...todo..写JNI回调，告诉java 层，通知用户
        return;
    }

    //在这 媒体上下文，就有了丰富的值了 == avFormatContext

    //循环遍历，媒体格式里面的 流1==音频  流2==字幕  流0==视频

    for (int i = 0; i < this->avFormatContext->nb_streams; ++i) {

    }

}
