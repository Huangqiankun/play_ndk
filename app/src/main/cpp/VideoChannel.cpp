//
// Created by MC on 2021/5/12.
//


#include "VideoChannel.h"

VideoChannel::VideoChannel(int stream_index, AVCodecContext *avCodecContext)
        : BaseChannel(stream_index, avCodecContext) {

}

VideoChannel::~VideoChannel() {

}

void *task_video_decode(void *pVoid) {
    VideoChannel *videoChannel = static_cast<VideoChannel *>(pVoid);
    videoChannel->video_decode_();
    return 0;

}

void *task_video_play(void *pVoid) {
    VideoChannel *videoChannel = static_cast<VideoChannel *>(pVoid);
    videoChannel->video_play_();

    return 0;
}


// 从队列里面取出，未解码的数据
//1.解码
//2.播放
void VideoChannel::start() {
    //存放未解码的队列，开始工作了
    packets.setFlag(1);
    //存放已解码的队列，开始工作了
    frames.setFlag(1);
    //解码的线程
    pthread_create(&pid_video_decode, 0, task_video_decode, this);

    //播放的线程
    pthread_create(&pid_video_play, 0, task_video_play, this);
}

void VideoChannel::stop() {

}

//异步，真正的解码
void VideoChannel::video_decode_() {
    //取出 未解码的队列数据
    AVPacket *packet;
    while (isPlaying) {
        int ret = this->packets.pop(packet);
        if (!isPlaying) {
            break;
        }
        if (!ret) {
            continue;
        }
        //走到这里，就证明了，取到了，未解码的视频数据包
        ret = avcodec_send_packet(this->avCodecContext, packet);
        if (ret) {
            break; //失败了
        }
        AVFrame *avFrame = av_frame_alloc();
        ret = avcodec_receive_frame(this->avCodecContext, avFrame);
        if (ret == AVERROR(EAGAIN)) {
            //代表 帧去的不完整，I帧 （完整的一帧），如果是P帧 B帧 就会进入这里面，证明没有取完整
            continue;//直到取到完整的帧位置
        } else if (ret != 0) {
            // todo 以后做释放工作
            break;
        }
        //终于取到了原始数据
        this->frames.push(avFrame);
    }
    // todo 出了循环，释放

}


//异步，真正的播放
void VideoChannel::video_play_() {
    // yuv最原始的视频帧数据，不能显示到屏幕上（Android，iOS，SDL）
    // yuv ---> rgba（才能真正的显示到屏幕上）
    // 转化的上下文

    SwsContext * swsContext = sws_getContext(
            // 原始的一层  宽 高 格式
            this->avCodecContext->width, this->avCodecContext->height, this->avCodecContext->pix_fmt
            ,
            // 目标 最终要显示到屏幕的信息，最好和原始的保持一致
            this->avCodecContext->width, this->avCodecContext->height, AV_PIX_FMT_RGBA,

            SWS_BILINEAR, NULL, NULL, NULL
    );

    AVFrame * frame = 0;

    while (isPlaying) {
        int ret = frames.pop(frame);


    }
}