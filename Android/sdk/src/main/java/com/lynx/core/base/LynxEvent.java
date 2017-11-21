// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.core.base;

public class LynxEvent extends LynxObject implements Coalescence {

    protected static final String TYPE = "type";
    protected String mType;

    public LynxEvent(String type) {
        mType = type;
        setProperty(TYPE, type);
    }

    public boolean canCoalesce() {
        return false;
    }

    public int getCoalescingKey() {
        return 0;
    }
}
