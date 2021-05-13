package com.hqk.play_ndk;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;

public class Constants {

    public enum ErrorCode {
        FFMPEG_CAN_NOT_OPEN_URL(-1, "打不开媒体数据源"),
        FFMPEG_CAN_NOT_FIND_STREAMS(-2, "找不到媒体流信息"),
        FFMPEG_FIND_DECODER_FAIL(-3, "找不到解码器"),
        FFMPEG_ALLOC_CODEC_CONTEXT_FAIL(-4, "无法根据解码器创建上下文"),
        FFMPEG_CODEC_CONTEXT_PARAMETERS_FAIL(-5, "根据流信息 配置上下文参数失败"),
        FFMPEG_OPEN_DECODER_FAIL(-6, "打开解码器失败"),
        FFMPEG_NOMEDIA(-7, "没有音视频"),
        FFMPEG_READ_PACKETS_FAIL(-8, "读取媒体数据包失败");
//        OTHER_ERROR(-9, "其他错误");


        private String value;
        private int code;

        public String getValue() {
            return value;
        }

        public int getCode() {
            return code;
        }

        ErrorCode(int code, String value) {
            this.code = code;
            this.value = value;
        }


        public static ErrorCode codeOf(int code) throws Exception {
            for (ErrorCode errorCode : values()) {
                if (errorCode.getCode() == code) {
                    return errorCode;
                }
            }
            throw new Exception("其他错误");

        }
    }
}
