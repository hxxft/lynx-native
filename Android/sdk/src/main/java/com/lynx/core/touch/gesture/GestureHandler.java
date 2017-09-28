// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.core.touch.gesture;

import android.content.Context;
import android.util.Pair;
import android.view.GestureDetector;
import android.view.MotionEvent;

import java.util.ArrayList;
import java.util.List;

// TODO: 17/9/28 Support multiple pointer, such as zooming
public class GestureHandler extends GestureDetector.SimpleOnGestureListener {
    private GestureDetector mGestureDetector;
    private GestureEventInfo mInfo;
    private List<GestureWatcher> mWatcherList;

    public GestureHandler(Context context) {
        mGestureDetector = new GestureDetector(context, this);
        mInfo = new GestureEventInfo();
        mWatcherList = new ArrayList<>();
    }

    public void handle(MotionEvent motionEvent) {
        updateGestureType(GestureEventInfo.NOTHING);
        mGestureDetector.onTouchEvent(motionEvent);
    }

    public void reset() {
        mInfo.reset();
        mWatcherList.clear();
    }

    public void addGestureWatcher(GestureWatcher watcher) {
        mWatcherList.add(watcher);
    }

    @Override
    public boolean onSingleTapUp(MotionEvent e) {
        updateGestureType(GestureEventInfo.CLICK);
        return super.onSingleTapUp(e);
    }

    @Override
    public boolean onFling(MotionEvent e1, MotionEvent e2, float velocityX, float velocityY) {

        updateGestureType(GestureEventInfo.FLING,
                new Pair<String, Object>("velocityX", velocityX),
                new Pair<String, Object>("velocityY", velocityY));
        return super.onFling(e1, e2, velocityX, velocityY);
    }

    @Override
    public boolean onDoubleTap(MotionEvent e) {
        updateGestureType(GestureEventInfo.DOUBLE_TAP);
        return super.onDoubleTap(e);
    }

    @Override
    public void onLongPress(MotionEvent e) {
        updateGestureType(GestureEventInfo.LONG_PRESS);
        super.onLongPress(e);
    }


    private void updateGestureType(String type, Pair<String, Object>... pairs) {
        mInfo.setType(type);
        if (pairs != null) {
            for (Pair<String, Object> pair : pairs) {
                mInfo.addDetail(pair.first, pair.second);
            }
        }
        if (!type.equals(GestureEventInfo.NOTHING)) {
            notifyWatcher();
        }
    }

    private void notifyWatcher() {
        for (int i = mWatcherList.size() - 1; i >= 0; --i) {
            GestureWatcher watcher = mWatcherList.get(i);
            watcher.performGesture(mInfo);
            if (watcher.isStopPropagation()) {
                break;
            }
        }
    }

}
