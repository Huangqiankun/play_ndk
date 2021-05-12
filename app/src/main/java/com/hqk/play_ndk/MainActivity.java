package com.hqk.play_ndk;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.view.SurfaceView;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {

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
}
