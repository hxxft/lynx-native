// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.core.base;

public class LynxEvent extends LynxMap implements Coalescence {

    protected static final String TYPE = "type";
    protected String mType;
    private LynxHolder mTarget;

    public LynxEvent(String type) {
        mType = type;
        set(TYPE, type);
    }

    public void setTarget(LynxHolder target) {
        mTarget = target;
        set("target", target);
    }

    public boolean canCoalesce() {
        return false;
    }

    public int getCoalescingKey() {
        return 0;
    }
}
