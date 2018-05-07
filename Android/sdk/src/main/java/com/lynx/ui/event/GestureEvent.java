// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.ui.event;

import com.lynx.core.base.LynxEvent;
import com.lynx.core.base.LynxMap;
import com.lynx.core.touch.gesture.GestureEventInfo;
import com.lynx.ui.LynxUI;

/* package */ class GestureEvent extends LynxEvent {

    private final static String KEY_GESTURE = "gesture";
    private final static String KEY_GESTURE_DETAIL = "detail";
    private final static String KEY_GESTURE_TYPE = "type";
    private String mGestureType;

    public GestureEvent(GestureEventInfo info) {
        super(KEY_GESTURE);
        mGestureType = info.getType();
        setTarget(((LynxUI) info.getCurTarget()).getRenderObjectImpl());

        LynxMap gesture = new LynxMap();
        LynxMap detail = new LynxMap();
        detail.set(KEY_GESTURE_TYPE, mGestureType);
        if (info.getDetail() != null) {
            for (String key : info.getDetail().keySet()) {
                detail.set(key, info.getDetail().get(key));
            }
        }

        gesture.set(KEY_GESTURE_DETAIL, detail);
        this.set(KEY_GESTURE, gesture);
    }

    @Override
    public boolean canCoalesce() {
        return true;
    }

    @Override
    public int getCoalescingKey() {
        return mGestureType.hashCode();
    }
}
