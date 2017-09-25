// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.ui.button;

import android.content.Context;
import android.support.v7.widget.AppCompatButton;

import com.lynx.base.Style;
import com.lynx.core.impl.RenderObjectImpl;
import com.lynx.ui.LynxUI;

public class LynxUIButton extends LynxUI<AppCompatButton> {

    public LynxUIButton(Context context, RenderObjectImpl impl) {
        super(context, impl);
    }

    @Override
    protected AppCompatButton createView(Context context) {
        return new AppCompatButton(context);
    }

    @Override
    public void updateStyle(Style style) {
//        super.updateStyle(style);
        mView.setPadding(0, 0, 0, 0);
    }

    @Override
    public void setText(String text) {
        mView.setText(text);
    }
}
