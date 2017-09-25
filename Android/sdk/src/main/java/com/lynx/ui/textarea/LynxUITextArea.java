// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.ui.textarea;

import android.content.Context;
import android.support.annotation.NonNull;
import android.view.Gravity;

import com.lynx.base.Style;
import com.lynx.core.impl.RenderObjectImpl;
import com.lynx.ui.input.AndroidInput;
import com.lynx.ui.input.LynxUIInput;

public class LynxUITextArea extends LynxUIInput {

    public LynxUITextArea(Context context, RenderObjectImpl impl) {
        super(context, impl);
    }

    @Override
    protected AndroidInput createView(Context context) {
        AndroidInput input = super.createView(context);
        input.setMaxLines(Integer.MAX_VALUE);
        input.setGravity(Gravity.TOP);
        return input;
    }

    @Override
    public void updateStyle(Style style) {
        super.updateStyle(style);
        if (style != null) {
            setWhiteSpace(style);
        }
    }

    public void setWhiteSpace(@NonNull Style style) {
        if (style.mWhiteSpace == Style.CSSTEXT_WHITESPACE_NOWRAP) {
            mView.setSingleLine();
        } else {
            mView.setSingleLine(false);
        }
    }
}
