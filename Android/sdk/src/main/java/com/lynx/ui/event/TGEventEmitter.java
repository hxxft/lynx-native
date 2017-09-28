// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.ui.event;

import com.lynx.core.touch.TouchEventInfo;
import com.lynx.core.touch.gesture.GestureEventInfo;
import com.lynx.ui.LynxUI;

import java.util.HashMap;
import java.util.Map;

// Touch and Gesture event emitter
public class TGEventEmitter {
    private LynxUI mTarget;
    private Map<String, Boolean> mEventStore;

    public TGEventEmitter(LynxUI ui) {
        mTarget = ui;
        mEventStore = new HashMap<>();
    }

    public void setEnable(String type, boolean enable) {
        switch (type) {
            case TouchEventInfo.START:
            case TouchEventInfo.MOVE:
            case TouchEventInfo.END:
            case TouchEventInfo.CANCEL:
            case GestureEventInfo.CLICK:
            case GestureEventInfo.LONG_PRESS:
            case GestureEventInfo.DOUBLE_TAP:
            case GestureEventInfo.FLING:
                if (enable) {
                    mEventStore.put(type, enable);
                } else  {
                    mEventStore.remove(type);
                }
                break;
            default:
                break;
        }
    }

    public void emitTouchEvent(TouchEventInfo info) {
        if (mEventStore.get(info.getType()) != null) {
            TouchEvent touchEvent = new TouchEvent(info.getType(), info);
            mTarget.postEvent(info.getType(), touchEvent);
        }
    }

    public void emitGestureEvent(GestureEventInfo info) {
        if (mEventStore.get(info.getType()) != null) {
            GestureEvent gestureEvent = new GestureEvent(info);
            mTarget.postEvent(info.getType(), gestureEvent);
        }
    }
}
