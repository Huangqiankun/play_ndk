package com.hqk.play_ndk;

import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.view.SurfaceView;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity implements MyPlayer.OnPreparedListener {

    private SurfaceView surfaceView;
    private final String PATH = "rtmp://58.200.131.2:1935/livetv/hbtv";
    private MyPlayer myPlayer;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        surfaceView = findViewById(R.id.surfaceView);
        initPlayer();
    }

    private void initPlayer() {
        myPlayer = new MyPlayer();
        myPlayer.setDataSource(PATH);
        myPlayer.setSurfaceView(surfaceView);
        myPlayer.setOnPreparedListener(this);
    }

    @Override
    protected void onResume() {
        super.onResume();
        myPlayer.prepare();
    }

    @Override
    protected void onPause() {
        super.onPause();
        myPlayer.stop();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        myPlayer.release();
    }

    @Override
    public void onlPrepared() {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                new AlertDialog.Builder(MainActivity.this)
                        .setTitle("UI")
                        .setMessage("准备好了，开始播放 ...")
                        .setPositiveButton("老夫知道了", null)
                        .show();
            }
        });

//        myPlayer.start(); // 开始播放
    }

    @Override
    public void onError(final String errorText) {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                new AlertDialog.Builder(MainActivity.this)
                        .setTitle("Error")
                        .setMessage("已经发生错误，请查阅:" + errorText)
                        .setPositiveButton("我来个去，什么情况", null)
                        .show();
            }
        });
    }
}
