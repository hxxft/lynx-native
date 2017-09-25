// Copyright 2017 The Lynx Authors. All rights reserved.

package com.lynx.core.touch;

import com.lynx.core.base.LynxArray;

public class TouchEventInfo extends LynxArray {

    private long mTimeStamp;
    private TouchAxis mTouchAxis;
    private TouchEventType mTouchEventType;
    private MotionEventType mMotionType;

    public TouchEventInfo(float x, float y, long timeStamp,
                          TouchEventType touchEventType, MotionEventType motionType) {
        this.mTouchAxis = new TouchAxis();
        this.mTouchAxis.x = x;
        this.mTouchAxis.y = y;
        this.mTimeStamp = timeStamp;
        this.mTouchEventType = touchEventType;
        this.mMotionType = motionType;

        add(mTouchAxis.x);
        add(mTouchAxis.y);
        add(mTimeStamp);
        add(mTouchEventType.getEventName());
        add(mMotionType.getEventName());
    }

    public long getTimeStamp() {
        return mTimeStamp;
    }

    public float getX() {
        return mTouchAxis.x;
    }
    public float getY() {
        return mTouchAxis.y;
    }

    public String getTouchEventType() {
        return mTouchEventType.getEventName();
    }

    public String getMotionType() {
        return mMotionType.getEventName();
    }
}
