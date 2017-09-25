// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.base;

import android.annotation.TargetApi;
import android.os.Build;
import android.view.Choreographer;

public class FrameRateController {
    private VSyncListener mListener;
    private final Choreographer mChoreographer;
    private final Choreographer.FrameCallback mVSyncFrameCallback;

    public interface VSyncListener {
        void OnVSync();
    }

    @TargetApi(Build.VERSION_CODES.JELLY_BEAN)
    public FrameRateController(VSyncListener listener) {
        mListener = listener;
        mChoreographer = Choreographer.getInstance();
        mVSyncFrameCallback = new Choreographer.FrameCallback() {
            @Override
            public void doFrame(long frameTimeNanos) {
                mChoreographer.postFrameCallback(mVSyncFrameCallback);
                mListener.OnVSync();
            }
        };

    }

    @TargetApi(Build.VERSION_CODES.JELLY_BEAN)
    public void start() {
        mChoreographer.postFrameCallback(mVSyncFrameCallback);
    }

    @TargetApi(Build.VERSION_CODES.JELLY_BEAN)
    public void stop() {
        mChoreographer.removeFrameCallback(mVSyncFrameCallback);
    }

}
