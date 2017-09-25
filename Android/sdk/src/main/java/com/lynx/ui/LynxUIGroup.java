// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.ui;

import android.content.Context;
import android.view.ViewGroup;

import com.lynx.core.impl.RenderObjectImpl;

public abstract class LynxUIGroup<T extends ViewGroup>
        extends LynxUI<T> {

    public LynxUIGroup(Context context) {
        super(context);
    }

    public LynxUIGroup(Context context, RenderObjectImpl impl) {
        super(context, impl);
    }

    @Override
    public abstract void insertChild(RenderObjectImpl child, int index);

    @Override
    public abstract void removeChild(RenderObjectImpl child);

    /**
     * Generating a suitable LayoutParams for child.
     *
     * @param childParams childParams to check if child ui need
     *                    a new LayoutParams
     * @return LayoutParams, will be set into Android View if it
     * is not the same as childParams
     */
    public abstract ViewGroup.LayoutParams generateLayoutParams(
            ViewGroup.LayoutParams childParams);

    @Override
    public void measure() {
        measureChildren();
        super.measure();
    }

    public void measureChildren() {
        for (int i = 0; i < mRenderObjectImpl.getChildCount(); ++i) {
            RenderObjectImpl child = mRenderObjectImpl.getChildAt(i);
            if (child.hasUI()) {
                child.getUI().measure();
            }
        }
    }

    @Override
    public void layout() {
        super.layout();
        layoutChildren();
    }

    public void layoutChildren() {
        for (int i = 0; i < mRenderObjectImpl.getChildCount(); ++i) {
            RenderObjectImpl child = mRenderObjectImpl.getChildAt(i);
            if (child.hasUI()) {
                child.getUI().layout();
            }
        }
    }
}
