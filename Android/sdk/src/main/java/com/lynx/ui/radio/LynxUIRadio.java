// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.ui.radio;

import android.content.Context;
import android.support.v7.widget.AppCompatRadioButton;

import com.lynx.core.impl.RenderObjectImpl;
import com.lynx.ui.LynxUI;

public class LynxUIRadio extends LynxUI<AppCompatRadioButton> {
    private final static String ATTR_CHECKED = "checked";

    public LynxUIRadio(Context context, RenderObjectImpl impl) {
        super(context, impl);
    }

    @Override
    protected AppCompatRadioButton createView(Context context) {
        return new AppCompatRadioButton(context);
    }

    @Override
    public void setText(String text) {
        mView.setText(text);
    }

    @Override
    public void setAttribute(String key, String value) {
        super.setAttribute(key, value);
        switch (key) {
            case ATTR_CHECKED:
                setChecked(mRenderObjectImpl);
                break;
            default:
                break;
        }
    }

    protected void setChecked(RenderObjectImpl impl) {
        mView.setChecked(Boolean.valueOf(impl.getAttribute(ATTR_CHECKED)));
    }

}
