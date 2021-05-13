
cmake_minimum_required(VERSION 3.4.1)

# 定义ffmpeg总目录的变量
set(FFMPEG ${CMAKE_SOURCE_DIR}/ffmpeg)

# 定义 rtmp总目录的变量
set(RTMP ${CMAKE_SOURCE_DIR}/rtmp)

# 指定ffmpeg里面的头文件目录
include_directories(${FFMPEG}/include)

# 指定ffmpeg里面的库文件目录（静态库）
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -L${FFMPEG}/libs/${CMAKE_ANDROID_ARCH_ABI}")

# 指定rtmp里面的库文件目录（静态库）
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -L${RTMP}/libs/${CMAKE_ANDROID_ARCH_ABI}")

file(GLOB allCpp *.cpp)

add_library(
        my-player
        SHARED

        ${allCpp})


find_library(
        log-lib

        log)



# 批量添加自己写的cpp文件
file(GLOB CPPFILS *.cpp) # 注意：这里有一个坑，不要把 *.h 加入进来了，例如：*.h

target_link_libraries(
        my-player
        -Wl,--start-group #在这个组合内，能够忽略加载顺序引发的问题
        avcodec avfilter avformat avutil swresample swscale
        -Wl,--end-group
        ${log-lib}
        z
        rtmp
        android
        OpenSLES
)