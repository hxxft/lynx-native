// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.ui.event;

import com.lynx.base.Style;
import com.lynx.core.base.LynxArray;
import com.lynx.core.base.LynxEvent;
import com.lynx.core.base.LynxObject;
import com.lynx.core.touch.TouchEventInfo;
import com.lynx.utils.PixelUtil;

/* package */ class TouchEvent extends LynxEvent {

    private final static String KEY_CLIENT_X = "clientX";
    private final static String KEY_CLIENT_Y = "clientY";
    private final static String KEY_TOUCHES = "touches";
    private final static String KEY_ACTION_INDEX = "actionIndex";
    private final static String KEY_POINTER_ID = "pointerId";

    private int mPointerID;

    public TouchEvent(String type, TouchEventInfo info) {
        super(type);

        mPointerID = info.getPointerID();

        LynxArray touches = new LynxArray();
        for (int i = 0; i < info.getPointerCount(); i++) {
            LynxObject touch = new LynxObject();
            touch.setProperty(KEY_CLIENT_X, PixelUtil.pxToLynxNumber(info.getX(i)));
            touch.setProperty(KEY_CLIENT_Y, PixelUtil.pxToLynxNumber(info.getY(i)));
            touches.add(touch);
        }
        setProperty(KEY_TOUCHES, touches);
        setProperty(KEY_ACTION_INDEX, info.getActionIndex());
        setProperty(KEY_POINTER_ID, info.getPointerID());
    }

    @Override
    public int getCoalescingKey() {
        return mPointerID;
    }

    @Override
    public boolean canCoalesce() {
        switch (mType) {
            case TouchEventInfo.START:
            case TouchEventInfo.END:
            case TouchEventInfo.CANCEL:
                return false;
            case TouchEventInfo.MOVE:
                return true;
            default:
                throw new RuntimeException("The touch event can not be recognized");
        }
    }
}
