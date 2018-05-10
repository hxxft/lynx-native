// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.core.touch;

import android.view.MotionEvent;

public class EventInfo<T extends EventTarget> {
    private boolean mCancelBubble;
    private boolean mPreventDefault;
    private int mPointerId;
    private String mType;
    private T mTarget;
    private T mCurrentTarget;

    private MotionEvent mAndroidEvent;

    public EventInfo(MotionEvent ev, String type) {
        this.mCancelBubble = false;
        this.mAndroidEvent = ev;
        this.mPointerId = ev.getPointerId(ev.getActionIndex());
        this.mType = type;
    }

    public void stopPropagation(){
        mCancelBubble = true;
    }

    public void preventDefault() {
        mPreventDefault = true;
    }

    public boolean getCancelBubble() {
        return mCancelBubble;
    }

    public boolean getPreventDefault() {
        return mPreventDefault;
    }

    public void reset() {
        mCancelBubble = false;
        mPreventDefault = false;
    }

    public MotionEvent getAndroidEvent() {
        return mAndroidEvent;
    }

    public int getPointerId() {
        return mPointerId;
    }

    public String getType() {
        return mType;
    }

    public void setType(String type) {
        mType = type;
    }

    public void setTarget(T target) {
        mTarget = target;
    }

    public T getTarget() {
        return mTarget;
    }

    public void setCurTarget(T target) {
        mCurrentTarget = target;
    }

    public T getCurTarget() {
        return mCurrentTarget;
    }
}
