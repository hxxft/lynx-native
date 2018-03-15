// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.test;

import android.app.Activity;
import android.os.SystemClock;
import android.view.MotionEvent;
import android.view.View;

public class SingleTouchGenerator {
    private View mDispatcherView;
    private long mLastDownTime;
    private long mEventTime;
    private float mLastX;
    private float mLastY;

    public SingleTouchGenerator(View view) {
        mDispatcherView = view;
    }

    public SingleTouchGenerator(Activity activity) {
        mDispatcherView = activity.getWindow().getDecorView();
    }

    public void down(float x, float y) {
        dispatchEvent(MotionEvent.ACTION_DOWN, x, y, SystemClock.uptimeMillis());
    }

    public void up() {
        dispatchEvent(MotionEvent.ACTION_UP, mLastX, mLastY, SystemClock.uptimeMillis());
    }

    public void cancel() {
        dispatchEvent(MotionEvent.ACTION_CANCEL, mLastX, mLastY, SystemClock.uptimeMillis());
    }

    public void up(float x, float y) {
        dispatchEvent(MotionEvent.ACTION_UP, x, y, SystemClock.uptimeMillis());
    }

    public void move(float x, float y) {
        dispatchEvent(MotionEvent.ACTION_MOVE, x, y, SystemClock.uptimeMillis());
    }

    private void dispatchEvent(final int action, final float x, final float y, long eventTime) {
        mEventTime = eventTime;
        if (action == MotionEvent.ACTION_DOWN) {
            mLastDownTime = eventTime;
        }
        mLastX = x;
        mLastY = y;
        mDispatcherView.post(new Runnable() {
            @Override
            public void run() {
                MotionEvent event = MotionEvent.obtain(mLastDownTime,
                        mEventTime, action, x, y, 0);
                mDispatcherView.dispatchTouchEvent(event);
                event.recycle();
            }
        });
    }
}
