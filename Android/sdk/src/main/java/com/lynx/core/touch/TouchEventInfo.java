// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.core.touch;

import android.view.MotionEvent;

public class TouchEventInfo {

    public final static String START = "touchstart";
    public final static String MOVE = "touchmove";
    public final static String END = "touchend";
    public final static String CANCEL = "touchcancel";
    public final static String NOTHING = "nothing";

    private TouchAxis[] mTouchAxisList;
    private String mTouchType;
    private int mActionIndex;
    // Only for distinguishing pointer for START and END
    private int mPointerID;
    private int mPointerCount;

    private MotionEvent mAndroidEvent;

    public TouchEventInfo(MotionEvent event) {

        mAndroidEvent = event;

        String touchEventType = TouchEventInfo.NOTHING;
        int action = (event.getAction() & MotionEvent.ACTION_MASK);
        switch (action) {
            case MotionEvent.ACTION_DOWN:
            case MotionEvent.ACTION_POINTER_DOWN:
                touchEventType = TouchEventInfo.START;
                break;
            case MotionEvent.ACTION_UP:
            case MotionEvent.ACTION_POINTER_UP:
                touchEventType = TouchEventInfo.END;
                break;
            case MotionEvent.ACTION_MOVE:
                touchEventType = TouchEventInfo.MOVE;
                break;
            case MotionEvent.ACTION_CANCEL:
                touchEventType = TouchEventInfo.CANCEL;
                break;
            default:
                break;
        }

        mActionIndex = event.getActionIndex();
        mPointerID = event.getPointerId(event.getActionIndex());
        mPointerCount = event.getPointerCount();
        mTouchAxisList = new TouchAxis[event.getPointerCount()];
        for (int i = 0; i < event.getPointerCount(); i++) {
            mTouchAxisList[i] = new TouchAxis();
            mTouchAxisList[i].x = event.getX(i);
            mTouchAxisList[i].y = event.getY(i);
        }
        mTouchType = touchEventType;
    }

    public float getX() {
        return mTouchAxisList[mActionIndex].x;
    }

    public float getY() {
        return mTouchAxisList[mActionIndex].y;
    }

    public float getX(int index) {
        return mTouchAxisList[index].x;
    }

    public float getY(int index) {
        return mTouchAxisList[index].y;
    }

    public TouchAxis getAxis(int index) {
        return mTouchAxisList[index];
    }

    public TouchAxis getAxis() {
        return mTouchAxisList[mActionIndex];
    }

    public String getType() {
        return mTouchType;
    }

    public int getActionIndex() {
        return mActionIndex;
    }

    public int getPointerID() {
        return mPointerID;
    }

    public int getPointerCount() {
        return mPointerCount;
    }

    public MotionEvent getAndroidEvent() {
        return mAndroidEvent;
    }
}
