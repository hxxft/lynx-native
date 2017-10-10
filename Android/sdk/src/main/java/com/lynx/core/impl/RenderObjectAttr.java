// Copyright 2017 The Lynx Authors. All rights reserved.

package com.lynx.core.impl;


public enum RenderObjectAttr {
    SCROLL_TOP(0),
    SCROLL_LEFT(1),
    GET_TEXT(2),
    TEXT_LAYOUT(3),
    ANIMATE_PROPS(4);

    private int mValue;
    RenderObjectAttr(int value) {
        this.mValue = value;
    }

    public int value() {
        return mValue;
    }
}
