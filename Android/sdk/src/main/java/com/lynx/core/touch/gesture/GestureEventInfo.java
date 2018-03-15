// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.core.touch.gesture;

import com.lynx.core.touch.EventInfo;
import com.lynx.core.touch.TouchEventInfo;

import java.util.HashMap;
import java.util.Map;

public class GestureEventInfo extends EventInfo<GestureWatcher> {

    public static final String CLICK = "click";
    public static final String FLING = "fling";
    public static final String DOUBLE_TAP = "doubletap";
    public static final String LONG_PRESS = "longpress";
    public static final String NOTHING = "nothing";

    private Map<String, Object> mDetail;

    public GestureEventInfo(TouchEventInfo info) {
        super(info.getAndroidEvent(), info.getType());
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
        super.reset();
        if (mDetail != null) {
            mDetail.clear();
        }
        setType(NOTHING);
    }

}
