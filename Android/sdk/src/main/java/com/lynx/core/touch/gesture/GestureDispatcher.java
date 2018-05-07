// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.core.touch.gesture;

import android.content.Context;
import android.view.GestureDetector;
import android.view.MotionEvent;

import com.lynx.core.touch.TouchEventInfo;

import java.util.ArrayList;
import java.util.List;

// TODO: 17/9/28 Supports more gesture, such as zooming
public class GestureDispatcher extends GestureDetector.SimpleOnGestureListener {
    private TouchEventInfo mTouchEventInfo;
    private GestureDetector mGestureDetector;
    private List<GestureWatcher> mWatcherList;

    public GestureDispatcher(Context context) {
        mWatcherList = new ArrayList<>();
        mGestureDetector = new GestureDetector(context, this);
    }

    public void addWatcher(GestureWatcher watcher) {
        mWatcherList.add(watcher);
    }

    public void handle(TouchEventInfo touchEventInfo) {
        mTouchEventInfo = touchEventInfo;
        mGestureDetector.onTouchEvent(touchEventInfo.getAndroidEvent());
    }

    @Override
    public boolean onSingleTapUp(MotionEvent e) {
        GestureEventInfo info = new GestureEventInfo(mTouchEventInfo);
        info.setType(GestureEventInfo.CLICK);
        notifyWatcher(info);
        return super.onSingleTapUp(e);
    }

    @Override
    public boolean onFling(MotionEvent e1, MotionEvent e2, float velocityX, float velocityY) {
        GestureEventInfo info = new GestureEventInfo(mTouchEventInfo);
        info.setType(GestureEventInfo.FLING);
        info.addDetail("velocityX", velocityX);
        info.addDetail("velocityY", velocityY);
        notifyWatcher(info);
        return super.onFling(e1, e2, velocityX, velocityY);
    }

    @Override
    public boolean onDoubleTap(MotionEvent e) {
        GestureEventInfo info = new GestureEventInfo(mTouchEventInfo);
        info.setType(GestureEventInfo.DOUBLE_TAP);
        notifyWatcher(info);
        return super.onDoubleTap(e);
    }

    @Override
    public void onLongPress(MotionEvent e) {
        GestureEventInfo info = new GestureEventInfo(mTouchEventInfo);
        info.setType(GestureEventInfo.LONG_PRESS);
        notifyWatcher(info);
        super.onLongPress(e);
    }

    protected void notifyWatcher(GestureEventInfo info) {
        if (mWatcherList.size() > 0) {

            info.setTarget(mWatcherList.get(mWatcherList.size() - 1));

            for (int i = 0; i < mWatcherList.size(); ++i) {
                GestureWatcher watcher = mWatcherList.get(i);
                info.setCurTarget(watcher);
                watcher.onCaptureGestureEvent(info);
                if (info.getCancelBubble()) {
                    break;
                }
            }

            if (!info.getCancelBubble()) {
                for (int i = mWatcherList.size() - 1; i >= 0; --i) {
                    GestureWatcher watcher = mWatcherList.get(i);
                    info.setCurTarget(watcher);
                    watcher.performGesture(info);
                    if (info.getCancelBubble()) {
                        break;
                    }
                }
            }

        }
    }

    public void reset() {
        mWatcherList.clear();
    }

}
