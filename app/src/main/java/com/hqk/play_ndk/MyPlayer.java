package com.hqk.play_ndk;

import android.content.Context;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

import androidx.annotation.NonNull;

public class MyPlayer implements SurfaceHolder.Callback {

    private String dataSource;//rtmp网络直播流，本地流

    private OnPreparedListener onPreparedListener;
    private SurfaceHolder surfaceHolder;

    static {
        System.loadLibrary("my-player");
    }

    public MyPlayer() {


    }

    public void setDataSource(String dataSource) {
        this.dataSource = dataSource;
    }


    public void setSurfaceView(SurfaceView surfaceView) {
        //每次设置surfaceHolder 的时候，判断 surfaceHolder 释放被清除
        if (null != surfaceHolder) {
            surfaceHolder.removeCallback(this);
        }
        surfaceHolder = surfaceView.getHolder();
        surfaceHolder.addCallback(this);
    }

    @Override
    public void surfaceCreated(@NonNull SurfaceHolder surfaceHolder) {
    }

    @Override
    public void surfaceChanged(@NonNull SurfaceHolder surfaceHolder, int i, int i1, int i2) {
        setSurfaceNative(surfaceHolder.getSurface());
    }

    @Override
    public void surfaceDestroyed(@NonNull SurfaceHolder surfaceHolder) {
    }

    public void setOnPreparedListener(OnPreparedListener onPreparedListener) {
        this.onPreparedListener = onPreparedListener;
    }

    /**
     * 上层区域
     */

    public void prepare() {
        prepareNative(this.dataSource);
    }

    public void start() {
        startNative();
    }


    public void stop() {
        stopNative();
    }

    public void release() {
        releaseNative();
    }


    /**
     * native 区域
     */

    public native void prepareNative(String dataSource);

    public native void startNative();

    public native void stopNative();

    public native void releaseNative();

    public native void setSurfaceNative(Surface surface);    //告诉底层，上层可以怎么渲染，其实底层操控 surface 对象

    public native String getFFmpegVersion();

    /**
     * 给native 调用的函数
     */

    public void onPrepared() {
        if (onPreparedListener != null) {
            this.onPreparedListener.onlPrepared();
        }

    }

    public void onError(int errorCode) {
        if (onPreparedListener != null) {
            String errorText = null;
            try {
                Constants.ErrorCode code = Constants.ErrorCode.codeOf(errorCode);
                errorText = code.getValue();
            } catch (Exception e) {
                e.printStackTrace();
                errorText = e.getMessage();
            }
            this.onPreparedListener.onError(errorText);
        }
    }


    //通知 activity的接口
    interface OnPreparedListener {

        void onlPrepared();

        void onError(String errorText);

    }

}
