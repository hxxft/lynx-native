// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.ui.recycler;

import com.lynx.core.impl.RenderObjectImpl;

public class RecyclableItem {
    private RecyclableState mState = RecyclableState.Immutable;
    private RenderObjectImpl mRenderObjectImpl;
    private int mTop = 0;
    private int mBottom = 0;
    private int mLeft = 0;
    private int mRight = 0;

    public RecyclableItem(RenderObjectImpl shadowNode, RecyclableState state) {
        this.mRenderObjectImpl = shadowNode;
        this.mState = state;
    }

    public RenderObjectImpl getRenderObjectImpl() {
        return mRenderObjectImpl;
    }

    public RecyclableState getState() {
        return mState;
    }

    public void updatePosition(int scrollY, int scrollX) {
        this.mTop = scrollY;
        this.mBottom = mTop + mRenderObjectImpl.getPosition().getHeight();
        this.mLeft = scrollX;
        this.mRight = mLeft + mRenderObjectImpl.getPosition().getWidth();
    }

    public int getTop() {
        return mTop;
    }

    public int getBottom() {
        return mBottom;
    }

    public int getLeft() {
        return mLeft;
    }

    public int getRight() {
        return mRight;
    }
}
