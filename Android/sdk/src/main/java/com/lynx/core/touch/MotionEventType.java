// Copyright 2017 The Lynx Authors. All rights reserved.

package com.lynx.core.touch;


public enum MotionEventType {
    NOTHING(""),
    CLICK("click");

    private String mName;
    MotionEventType(String name) {
        this.mName = name;
    }

    public String getEventName() {
        return mName;
    }

}
