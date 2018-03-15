// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.core.touch;

import android.util.SparseArray;
import android.view.MotionEvent;

public class TouchEventInfo extends EventInfo<TouchTarget> {

    public final static String START = "touchstart";
    public final static String MOVE = "touchmove";
    public final static String END = "touchend";
    public final static String CANCEL = "touchcancel";

    private TouchTarget mCurTarget;

    private long mTimeStamp;
    private TouchAxis mTouchAxis;
    private TouchAxis mRawTouchAxis;
    private TouchHandler mTouchHandler;

    /* package */ TouchEventInfo(TouchHandler handler, MotionEvent ev,
                                 float x, float y, float rawX, float rawY,
                                 long timeStamp, String type) {
        super(ev, type);
        this.mTouchHandler = handler;
        this.mTouchAxis = new TouchAxis(x, y);
        this.mRawTouchAxis = new TouchAxis(rawX, rawY);
        this.mTimeStamp = timeStamp;
    }

    /* package */ boolean updateInfo(float x, float y, float rawX, float rawY,
                                     long timeStamp, String type) {
        mTimeStamp = timeStamp;
        setType(type);
        if (x == mTouchAxis.x && y == mTouchAxis.y
                && rawX == mRawTouchAxis.x && rawY == mRawTouchAxis.y) return false;
        mTouchAxis.x = x;
        mTouchAxis.y = y;
        mRawTouchAxis.x = rawX;
        mRawTouchAxis.y = rawY;
        return true;
    }

    public float getX() {
        return mTouchAxis.x;
    }

    public float getY() {
        return mTouchAxis.y;
    }

    public float getRawX() {
        return mRawTouchAxis.x;
    }

    public float getRawY() {
        return mRawTouchAxis.y;
    }

    public TouchAxis getTouchAxis() {
        return mTouchAxis;
    }

    public long getTimeStamp() {
        return mTimeStamp;
    }

    public void setCurTarget(TouchTarget curTarget) {
        mCurTarget = curTarget;
    }

    public TouchTarget getCurTarget() {
        return mCurTarget;
    }

    public SparseArray<TouchEventInfo> getInfoListOnScreen() {
        return mTouchHandler.infoOnScreen;
    }

    public SparseArray<TouchEventInfo> getChangedInfoList() {
        return mTouchHandler.changedInfoList;
    }

}
