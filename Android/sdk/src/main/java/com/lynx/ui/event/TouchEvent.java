// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.ui.event;

import android.util.SparseArray;

import com.lynx.core.base.LynxArray;
import com.lynx.core.base.LynxEvent;
import com.lynx.core.base.LynxHolder;
import com.lynx.core.base.LynxObject;
import com.lynx.core.impl.RenderObjectImpl;
import com.lynx.core.touch.TouchEventInfo;
import com.lynx.ui.LynxUI;
import com.lynx.utils.PixelUtil;

/* package */ class TouchEvent extends LynxEvent {

    private final static String KEY_CHANGED_TOUCHES = "changedTouches";
    private final static String KEY_TOUCHES = "touches";
    private final static String KEY_CLIENT_X = "clientX";
    private final static String KEY_CLIENT_Y = "clientY";
    private final static String KEY_SCREEN_X = "screenX";
    private final static String KEY_SCREEN_Y = "screenY";
    private final static String KEY_SRC_ELEMENT = "srcElement";

    public long timeStamp;

    public TouchEvent(TouchEventInfo info) {
        super(info.getType());
        init(info);
    }

    private void init(TouchEventInfo info) {
        timeStamp = info.getTimeStamp();
        initChangedTouches(info);
        initTouches(info);
        setTarget(((LynxUI) info.getCurTarget()).getRenderObjectImpl());
        setSrcElement(((LynxUI) info.getTarget()).getRenderObjectImpl());
    }

    private void setSrcElement(LynxHolder srcElement) {
        setProperty(KEY_SRC_ELEMENT, srcElement);
    }

    private void initChangedTouches(TouchEventInfo info) {
        // Add extra property
        LynxArray changedTouches = new LynxArray();
        setProperty(KEY_CHANGED_TOUCHES, changedTouches);
        SparseArray<TouchEventInfo> infoList = info.getChangedInfoList();
        for (int i = 0; i < infoList.size(); ++i) {
            info = infoList.valueAt(i);
            LynxObject axis = new LynxObject();
            setClientAxis(axis,
                    (float) PixelUtil.pxToLynxNumber(info.getX()),
                    (float) PixelUtil.pxToLynxNumber(info.getY()));
            setScreenAxis(axis,
                    (float) PixelUtil.pxToLynxNumber(info.getRawX()),
                    (float) PixelUtil.pxToLynxNumber(info.getRawY()));
            changedTouches.add(axis);
        }
    }

    private void initTouches(TouchEventInfo info) {
        // Add extra property
        LynxArray touches = new LynxArray();
        setProperty(KEY_TOUCHES, touches);
        SparseArray<TouchEventInfo> infoList = info.getInfoListOnScreen();
        for (int i = 0; i < infoList.size(); ++i) {
            info = infoList.valueAt(i);
            LynxObject axis = new LynxObject();
            setClientAxis(axis,
                    (float) PixelUtil.pxToLynxNumber(info.getX()),
                    (float) PixelUtil.pxToLynxNumber(info.getY()));
            setScreenAxis(axis,
                    (float) PixelUtil.pxToLynxNumber(info.getRawX()),
                    (float) PixelUtil.pxToLynxNumber(info.getRawY()));
            touches.add(axis);
        }
    }

    private void setClientAxis(LynxObject axis, float x, float y) {
        axis.setProperty(KEY_CLIENT_X, x);
        axis.setProperty(KEY_CLIENT_Y, y);
    }

    private void setScreenAxis(LynxObject axis, float x, float y) {
        axis.setProperty(KEY_SCREEN_X, x);
        axis.setProperty(KEY_SCREEN_Y, y);
    }

    @Override
    public int getCoalescingKey() {
        return 1;
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
