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

        // Insert View
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
