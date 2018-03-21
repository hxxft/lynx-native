// Copyright 2017 The Lynx Authors. All rights reserved.

package com.lynx.core.impl;

import com.lynx.base.Position;
import com.lynx.base.Size;
import com.lynx.base.Style;
import com.lynx.ui.anim.AnimInfo;

import java.util.List;


public interface RenderImplInterface {
    void insertChild(RenderObjectImpl child, int index);
    void removeChild(RenderObjectImpl child);
    void requestLayout();
    void setText(String text);
    void setPosition(Position position);
    void setAttribute(String key, String value);
    void updateStyle(Style style);
    void setSize(Size size);
    void addEventListener(String event);
    void removeEventListener(String event);
    void setData(int attr, Object param);
    void animate(List<AnimInfo> infoList, AnimInfo.Option option);
    void cancelAnimation();
}
