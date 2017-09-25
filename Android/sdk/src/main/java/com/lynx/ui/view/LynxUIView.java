// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.ui.view;

import android.content.Context;
import android.view.View;
import android.view.ViewGroup;

import com.lynx.core.impl.RenderObjectImpl;
import com.lynx.ui.LynxUI;
import com.lynx.ui.LynxUIFactory;
import com.lynx.ui.LynxUIGroup;

public class LynxUIView extends LynxUIGroup<AndroidViewGroup> {

    public LynxUIView(Context context) {
        super(context);
    }

    public LynxUIView(Context context, RenderObjectImpl impl) {
        super(context, impl);
    }

    @Override
    protected AndroidViewGroup createView(Context context) {
        return new AndroidViewGroup(context, this);
    }

    @Override
    public void insertChild(RenderObjectImpl child, int index) {
        if (!child.hasUI()) {
            attachChildElement(child);
        }
        View impl = child.getUI().getView();
        if (impl.getParent() != null) {
            ((ViewGroup) impl.getParent()).removeView(impl);
        }
        mView.addView(impl, index);
    }

    /**
     * Recursively create the child element's view and insert to their parent
     * @param childElement
     */
    public void attachChildElement(RenderObjectImpl childElement) {
        LynxUI ui = LynxUIFactory.create(mView.getContext(), childElement);
        childElement.setUI(ui);
        for (int i = 0; i < childElement.getChildCount(); i++) {
            ui.insertChild(childElement.getChildAt(i), i);
        }
    }

    @Override
    public void removeChild(RenderObjectImpl child) {
        if ((child).hasUI()) {
            mView.removeView(child.getUI().getView());
        }
    }

    @Override
    public ViewGroup.LayoutParams generateLayoutParams(ViewGroup.LayoutParams childParams) {
        if (childParams != null) {
            return childParams;
        }
        return new ViewGroup.LayoutParams(0, 0);
    }

}
