// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.core.touch.gesture;

import java.util.HashMap;
import java.util.Map;

public class GestureEventInfo {

    public static final String CLICK = "click";
    public static final String FLING = "fling";
    public static final String DOUBLE_TAP = "doubletap";
    public static final String LONG_PRESS = "longpress";
    public static final String NOTHING = "nothing";

    private String mType;
    private Map<String, Object> mDetail;

    public GestureEventInfo() {
        mType = NOTHING;
    }

    public String getType() {
        return mType;
    }

    public void setType(String type) {
        mType = type;
    }

    public void addDetail(String key, Object value) {
        if (mDetail == null) {
            mDetail = new HashMap<>();
        }
        mDetail.put(key, value);
    }

    public Map<String, Object> getDetail() {
        return mDetail;
    }

    public void reset() {
        if (mDetail != null) {
            mDetail.clear();
        }
        mType = NOTHING;
    }
}
