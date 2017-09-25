// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.ui.body;

import android.content.Context;
import android.view.View;
import android.view.ViewGroup;

import com.lynx.base.SupposeUIThread;
import com.lynx.core.impl.RenderObjectImpl;
import com.lynx.core.tree.LynxUIAction;
import com.lynx.ui.LynxView;
import com.lynx.ui.view.AndroidViewGroup;
import com.lynx.ui.view.LynxUIView;

public class LynxUIBody extends LynxUIView {

    public LynxUIBody(Context context) {
        super(context);
    }

    @Override
    protected AndroidViewGroup createView(Context context) {
        // No-op as creating view is handled by {@Link LynxView}
        return null;
    }

    public void bindView(AndroidBody view) {
        mView = view;
    }

    @Override
    public void measure() {
        for (int i = 0; i < mRenderObjectImpl.getChildCount(); ++i) {
            if (mRenderObjectImpl.getChildAt(i).hasUI()) {
                mRenderObjectImpl.getChildAt(i).getUI().measure();
            }
        }
    }

    @Override
    public void layout() {
        for (int i = 0; i < mRenderObjectImpl.getChildCount(); ++i) {
            if (mRenderObjectImpl.getChildAt(i).hasUI()) {
                mRenderObjectImpl.getChildAt(i).getUI().layout();
            }
        }
    }

    @Override
    public void insertChild(RenderObjectImpl child, int i) {
        // Find the target index of thi child, and add to body view
        if (!child.hasUI()) {
            attachChildElement(child);
        }

        // Remove self from parent
        View childView = child.getUI().getView();
        ViewGroup parent = (ViewGroup) childView.getParent();
        if(parent != null) {
            parent.removeView(childView);
        }

        // Find the target position to insert
        int curZIndex = child.getStyle() == null ? 0 : child.getStyle().mZIndex;
        int nearestZIndex = Integer.MAX_VALUE;
        RenderObjectImpl nearestItem = null;
        for (int j = 0; j < mRenderObjectImpl.getChildCount(); j++) {
            RenderObjectImpl renderObjectImpl = mRenderObjectImpl.getChildAt(j);
            if (renderObjectImpl == null) {
                continue;
            }
            int tempZIndex = renderObjectImpl.getStyle() == null ?
                    0 : renderObjectImpl.getStyle().mZIndex;
            if (tempZIndex > curZIndex) {
                if (nearestZIndex > tempZIndex) {
                    nearestZIndex = tempZIndex;
                    nearestItem = renderObjectImpl;
                }
            }
        }

        if (nearestItem != null) {
            int index = mView.indexOfChild(nearestItem.getUI().getView());
            mView.addView(childView, index);
        } else {
            if (i < 0) {
                i = mRenderObjectImpl.getChildCount() - 1;
            }
            mView.addView(childView, i);
        }

    }

    @SupposeUIThread
    public void collect(LynxUIAction action) {
        ((LynxView) mView).getHostImpl().collect(action);
    }
}
