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

    @SupposeUIThread
    public void collect(LynxUIAction action) {
        ((LynxView) mView).getHostImpl().collect(action);
    }
}
