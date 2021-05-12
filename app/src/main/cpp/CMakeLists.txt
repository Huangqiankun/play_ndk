
cmake_minimum_required(VERSION 3.4.1)

file(GLOB allCpp *.cpp)

add_library(
        my-player
        SHARED

        ${allCpp})


find_library(
        log-lib

        log)

# 导入 FFmpeg 头文件
include_directories(${CMAKE_SOURCE_DIR}/ffmpeg/include)
#导入FFmpeg 库文件
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -L${CMAKE_SOURCE_DIR}/ffmpeg/libs/${CMAKE_ANDROID_ARCH_ABI}")
#RTMP的库文件
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -L${CMAKE_SOURCE_DIR}/rtmp/libs/${CMAKE_ANDROID_ARCH_ABI}")

target_link_libraries(
        my-player
        -Wl,--start-group #在这个组合内，能够忽略加载顺序引发的问题
        avcodec avfilter avformat avutil swresample swscale
        -Wl,--end-group
        ${log-lib}
        z
)