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

//void* (*__start_routine)(void*)
void *customTaskStartThread(void *pVoid) {
    MyPlayer *myPlayer = static_cast<MyPlayer *>(pVoid);
    myPlayer->start_();
    return 0;//必须返回，有坑！
}


MyPlayer::MyPlayer() {}

MyPlayer::~MyPlayer() {
    if (this->data_source) {
        delete this->data_source;
        this->data_source = 0;
    }
}

// 准备工作失去，“解码”，拆包裹（音视频，视频流，字幕流.....）
void MyPlayer::prepare() {
    //创建异步线程
    pthread_create(&pid_prepare, 0, customTaskPrepareThread, this);
}

//异步的函数
void MyPlayer::prepare_() {
    //拆包裹 --解复用，解封装，解码

    //获取生成上下文
    this->avFormatContext = avformat_alloc_context();

    // 字典：键值对的
    AVDictionary *dictionary = 0;
    av_dict_set(&dictionary, "timeout", "5000000", 0); // 单位是微妙

    // 是不是包裹，如果对方寄来的是 非包裹（被损坏的数据），那就没法玩了
    int ret = avformat_open_input(&avFormatContext, data_source, 0, &dictionary);
    LOGE("ret  %d",ret);
    LOGE("ret data_source %s ",data_source);
    //因为下面不需要字典，所以可回收字典
    av_dict_free(&dictionary);
    if (ret) {
        this->jniCallback->onErrorAction(THREAD_CHILD, FFMPEG_CAN_NOT_OPEN_URL);
        return;
    }
    //寻找，媒体格式中的，（音频，视频，字幕）
    ret = avformat_find_stream_info(this->avFormatContext, 0);//为什么第二个参数，不给字典，不需要设置格外配置
    if (ret < 0) {
        this->jniCallback->onErrorAction(THREAD_CHILD, FFMPEG_CAN_NOT_FIND_STREAMS);
        return;
    }

    //在这 媒体上下文，就有了丰富的值了 == avFormatContext

    //循环遍历，媒体格式里面的 流1==音频  流2==字幕  流0==视频
    for (int i = 0; i < this->avFormatContext->nb_streams; ++i) {
        //获取媒体流，音频，视频 压缩格式
        AVStream *stream = this->avFormatContext->streams[i];

        //拿到解码器ID，为了获取解码器
        AVCodecParameters *avCodecParameters = stream->codecpar;
        enum AVCodecID codecId = avCodecParameters->codec_id;
        // 拿到解码器
        AVCodec *codec = avcodec_find_decoder(codecId);
        if (!codec) {
            this->jniCallback->onErrorAction(THREAD_CHILD, FFMPEG_FIND_DECODER_FAIL);
            return;
        }
        //为了后续方便解码，还需要解码上下文
        AVCodecContext *codecContext = avcodec_alloc_context3(codec);
        if (!codecContext) {
            this->jniCallback->onErrorAction(THREAD_CHILD, FFMPEG_ALLOC_CODEC_CONTEXT_FAIL);
            return;
        }

        //还需要给解码器上下文设置参数
        ret = avcodec_parameters_to_context(codecContext, avCodecParameters);
        if (ret < 0) {
            this->jniCallback->onErrorAction(THREAD_CHILD, FFMPEG_CODEC_CONTEXT_PARAMETERS_FAIL);
            return;
        }
        // 打开解码器
        ret = avcodec_open2(codecContext, codec, 0);
        if (ret) {
            this->jniCallback->onErrorAction(THREAD_CHILD, FFMPEG_OPEN_DECODER_FAIL);
            return;
        }

        //解码器打开成功，就能使用解码器了
        //区分音频/视频  通道
        if (avCodecParameters->codec_type == AVMEDIA_TYPE_AUDIO) {//音频流
//            this->audioChannel = new AudioChannel(i, codecContext);
        } else if (avCodecParameters->codec_type == AVMEDIA_TYPE_VIDEO) {//视频流
            this->videoChannel = new VideoChannel(i, codecContext);
        } else {

        }


    }// 结束for 循环

    //告诉java 层，准备好了
    this->jniCallback->onPrepared(THREAD_CHILD);

}

MyPlayer::MyPlayer(const char *data_source, JNICallback *jniCallback) {
//    // 有坑，会报错，容易出现悬空指针
//    this->data_source=data_source;

//    //这里还是会有坑，长度  不对
//    this->data_source = new char[strlen(data_source)]

    this->jniCallback = jniCallback;
    //这样才对！
    this->data_source = new char[strlen(data_source) + 1];
    stpcpy(this->data_source, data_source);
}


void MyPlayer::start() {
    //开始播放，需要异步线程

    //让 音频 和 视频 两个通道 运行起来

    isPlayer = 1; //播放的状态
    if (this->videoChannel) {
        this->videoChannel->start();
    }

//    if (this->audioChannel) {
//        this->audioChannel->start();
//    }


    //开启线程 （读包：把未解码的流数据  音频、视频） 放入队列
    pthread_create(&pid_start, 0, customTaskStartThread, this);

}

//异步运行
void MyPlayer::start_() {
    //循环 读取视频包
    while (this->isPlayer) {
        //未解码的格式是保存在 AVPacket里面
        AVPacket *packet = av_packet_alloc();
        int ret = av_read_frame(this->avFormatContext, packet);
//        if (ret!=0){
//            return;
//        }
        if (!ret) {//ret == 0
            //把已经得到的Packet 放入队列中
            //先要判断是否是音频 还是 视频
            if (videoChannel && videoChannel->stream_index == packet->stream_index) {
                //说明是 视频包了
                this->videoChannel->packets.push(packet);
            }
        } else if (ret == AVERROR_EOF) {  // 文件末尾，读完了的意思
            // TODO 后续维护
        } else {
            // 代表失败了
            break;
        }
    }// end while

    //释放工作
    isPlayer=0;
    videoChannel->stop();
    // audioChannel->stop();
}

void MyPlayer::setRenderCallback(RenderCallback renderCallback) {
    this->renderCallback = renderCallback;
}


