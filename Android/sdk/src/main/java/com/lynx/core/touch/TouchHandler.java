// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.core.touch;

import android.util.SparseArray;
import android.view.MotionEvent;

/* package */ class TouchHandler {
    /* package */ SparseArray<TouchEventInfo> infoOnScreen = new SparseArray<>();
    /* package */ SparseArray<TouchEventInfo> changedInfoList = new SparseArray<>();

    public TouchEventInfo handleMotionEvent(MotionEvent motionEvent) {
        int action = (motionEvent.getAction() & MotionEvent.ACTION_MASK);
        TouchEventInfo info = null;
        int actionIndex = motionEvent.getActionIndex();
        int pointerId = motionEvent.getPointerId(actionIndex);
        int pointerCount = motionEvent.getPointerCount();
        changedInfoList.clear();
        float x = motionEvent.getX();
        float y = motionEvent.getY();
        float rawX = motionEvent.getRawX();
        float rawY = motionEvent.getRawY();
        // The relative offset of body to the screen
        float offsetX = rawX - x;
        float offsetY = rawY - y;
        long timeStamp =  motionEvent.getEventTime();
        // Action axis
        x = motionEvent.getX(actionIndex);
        y = motionEvent.getY(actionIndex);
        switch (action) {
            case MotionEvent.ACTION_DOWN:
            case MotionEvent.ACTION_POINTER_DOWN:
                info = new TouchEventInfo(this, motionEvent, x, y, x + offsetX, y + offsetY,
                        timeStamp, TouchEventInfo.START);
                infoOnScreen.put(pointerId, info);
                changedInfoList.put(pointerId, info);
                break;
            case MotionEvent.ACTION_UP:
            case MotionEvent.ACTION_POINTER_UP:
                info = infoOnScreen.get(pointerId);
                info.updateInfo(x, y, x + offsetX, y + offsetY, timeStamp, TouchEventInfo.END);
                infoOnScreen.remove(pointerId);
                changedInfoList.put(pointerId, info);
                break;
            case MotionEvent.ACTION_MOVE:
                for (int i = 0; i < pointerCount; ++i) {
                    pointerId = motionEvent.getPointerId(i);
                    info = infoOnScreen.get(pointerId);
                    x = motionEvent.getX(i);
                    y = motionEvent.getY(i);
                    if (info.updateInfo(x, y, x + offsetX, y + offsetY, timeStamp,
                            TouchEventInfo.MOVE)) {
                        changedInfoList.put(pointerId, info);
                    }
                }
                break;
            case MotionEvent.ACTION_CANCEL:
                for (int i = 0; i < infoOnScreen.size(); ++i) {
                    info = infoOnScreen.valueAt(i);
                    info.updateInfo(info.getX(), info.getY(), info.getRawX(), info.getRawY(),
                            timeStamp, TouchEventInfo.CANCEL);
                    changedInfoList.put(info.getPointerId(), info);
                }
                infoOnScreen.clear();
                break;
            default:
                break;
        }

        if (info != null) {
            info.reset();
        }

        return info;
    }

    public void reset() {
        infoOnScreen.clear();
        changedInfoList.clear();
    }
}
