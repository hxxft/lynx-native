// Copyright 2017 The Lynx Authors. All rights reserved.

package com.lynx.core.impl;


public enum RenderObjectAttr {
    SCROLL_TOP(0),
    SCROLL_LEFT(1),
    GET_TEXT(2),
    TEXT_LAYOUT(3),
    CANVAS_DRAW(4),
    CANVAS_APPEND(5),
    CANVAS_IMAGE_DATA(6);

    private int mValue;
    RenderObjectAttr(int value) {
        this.mValue = value;
    }

    public int value() {
        return mValue;
    }
}
