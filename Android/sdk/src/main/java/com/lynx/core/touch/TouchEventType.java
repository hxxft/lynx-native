// Copyright 2017 The Lynx Authors. All rights reserved.

package com.lynx.core.touch;

public enum TouchEventType {
    START("touchstart"),
    MOVE("touchmove"),
    END("touchend"),
    CANCEL("touchcancel"),
    NOTHING("notouch");

    private final String mEventName;

    TouchEventType(String eventName) {
        mEventName = eventName;
    }

    public String getEventName() {
        return mEventName;
    }

}
