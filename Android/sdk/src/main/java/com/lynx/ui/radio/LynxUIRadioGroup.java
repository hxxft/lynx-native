// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.ui.radio;

import android.content.Context;
import android.support.annotation.IdRes;
import android.support.v7.widget.AppCompatRadioButton;
import android.view.View;
import android.view.ViewGroup;
import android.widget.RadioGroup;

import com.lynx.core.base.LynxEvent;
import com.lynx.core.base.LynxObject;
import com.lynx.core.impl.RenderObjectImpl;
import com.lynx.ui.LynxUI;
import com.lynx.ui.LynxUIFactory;
import com.lynx.ui.LynxUIGroup;

public class LynxUIRadioGroup extends LynxUIGroup<AndroidRadioGroup> {
    private final static String EVENT_CHANGE = "change";

    public LynxUIRadioGroup(Context context, RenderObjectImpl impl) {
        super(context, impl);
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
        if (childParams instanceof RadioGroup.LayoutParams) {
            return childParams;
        }
        return new RadioGroup.LayoutParams(0, 0);
    }

    @Override
    protected AndroidRadioGroup createView(Context context) {
        return new AndroidRadioGroup(context);
    }

    @Override
    public void addEventListener(String event) {
        super.addEventListener(event);
        switch (event) {
            case EVENT_CHANGE:
                mView.setOnCheckedChangeListener(new InnerOnCheckedChangedListener());
                break;
            default:
                break;
        }
    }

    @Override
    public void removeEventListener(String event) {
        super.removeEventListener(event);
        switch (event) {
            case EVENT_CHANGE:
                mView.setOnCheckedChangeListener(null);
                break;
            default:
                break;
        }
    }

    private class InnerOnCheckedChangedListener
            implements RadioGroup.OnCheckedChangeListener {

        @Override
        public void onCheckedChanged(RadioGroup group, @IdRes int checkedId) {
            AppCompatRadioButton view = (AppCompatRadioButton) mView.findViewById(checkedId);
            if (view != null) {
                LynxEvent event = new LynxEvent(EVENT_CHANGE);
                LynxObject detail = new LynxObject();
                detail.setProperty("value", view.getText().toString());
                event.setProperty("detail", detail);
                postEvent(EVENT_CHANGE, event);
            }
        }
    }
}
