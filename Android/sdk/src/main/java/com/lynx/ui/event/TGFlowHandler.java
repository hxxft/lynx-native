// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.ui.event;

import com.lynx.core.impl.EventModifier;
import com.lynx.core.impl.RenderObjectImpl;
import com.lynx.core.touch.EventInfo;
import com.lynx.core.touch.TouchEventInfo;
import com.lynx.core.touch.gesture.GestureEventInfo;
import com.lynx.ui.LynxUI;

import java.util.Map;

// Touch and Gesture event flow handler
public class TGFlowHandler {
    private LynxUI mTarget;

    public TGFlowHandler(LynxUI ui) {
        mTarget = ui;
    }

    public void handleCaptureFlow(EventInfo info) {
        RenderObjectImpl renderObjectImpl = mTarget.getRenderObjectImpl();
        if (renderObjectImpl == null) return;

        String eventName = info.getType();
        Map<String, EventModifier> modifiers =
                renderObjectImpl.getEventManager().getOptionEvent(eventName);
        if (modifiers == null) {
            return;
        }
        for (String key : modifiers.keySet()) {
            EventModifier modifier = modifiers.get(key);
            if (modifier != null && modifier.isCapture()) {
                emitEvent(modifier.getOriginName(), info);
                if (modifier.isStop()) {
                    info.stopPropagation();
                }
                if (modifier.isPrevent()) {
                    info.preventDefault();
                }
            }
        }
    }

    public void handlePerformFlow(EventInfo info) {
        RenderObjectImpl renderObjectImpl = mTarget.getRenderObjectImpl();
        if (renderObjectImpl == null) return;

        String eventName = info.getType();
        Map<String, EventModifier> modifiers =
                renderObjectImpl.getEventManager().getOptionEvent(eventName);
        if (modifiers == null) {
            return;
        }
        for (String key : modifiers.keySet()) {
            EventModifier modifier = modifiers.get(key);
            if (!modifier.isCapture() && modifier.mockCall()
                    && !(modifier.isSelf() && info.getTarget() != mTarget)) {
                emitEvent(modifier.getOriginName(), info);
            }
            if (modifier.isStop()) {
                info.stopPropagation();
            }
            if (modifier.isPrevent()) {
                info.preventDefault();
            }
        }
    }

    private void emitEvent(String eventName, EventInfo info) {
        if (info instanceof TouchEventInfo) {
            emitTouchEvent(eventName, (TouchEventInfo) info);
        } else if (info instanceof GestureEventInfo) {
            emitGestureEvent(eventName, (GestureEventInfo) info);
        }
    }

    private void emitTouchEvent(String eventName, TouchEventInfo info) {
        TouchEvent touchEvent = new TouchEvent(info);
        mTarget.postEvent(eventName, touchEvent);
    }

    private void emitGestureEvent(String eventName, GestureEventInfo info) {
        GestureEvent gestureEvent = new GestureEvent(info);
        mTarget.postEvent(eventName, gestureEvent);
    }
}
