// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.core.impl;

public class EventModifier {
    private final static String PREVENT = "prevent";
    private final static String STOP = "stop";
    private final static String CAPTURE = "capture";
    private final static String ONCE = "once";
    private final static String SELF = "self";
    private boolean mIsPrevent;
    private boolean mIsStop;
    private boolean mIsCapture;
    private boolean mIsOnce;
    private boolean mIsSelf;
    private boolean mHasCalled;

    private String mEventName;
    private String mOriginName;

    public EventModifier(String event) {
        mOriginName = event;
        String[] result = event.split("\\.");
        mEventName = result[0];
        for (int i = 1; i < result.length; i++) {
            switch (result[i]) {
                case PREVENT:
                    mIsPrevent = true;
                    break;
                case STOP:
                    mIsStop = true;
                    break;
                case CAPTURE:
                    mIsCapture = true;
                    break;
                case ONCE:
                    mIsOnce = true;
                    break;
                case SELF:
                    mIsSelf = true;
                    break;
                default: break;
            }
        }
    }

    public boolean isPrevent() {
        return mIsPrevent;
    }

    public boolean isStop() {
        return mIsStop;
    }

    public boolean isCapture() {
        return mIsCapture;
    }

    public boolean isSelf() {
        return mIsSelf;
    }

    public boolean isOnce() {
        return mIsOnce;
    }

    public boolean mockCall() {
        if (isOnce() && mHasCalled) {
            return false;
        }
        mHasCalled = true;
        return true;
    }

    public String getName() {
        return mEventName;
    }

    public String getOriginName() {
        return mOriginName;
    }
}
