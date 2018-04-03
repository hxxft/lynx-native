// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.ui.label;

import android.content.Context;
import android.support.annotation.NonNull;
import android.text.Layout;

import com.lynx.base.Style;
import com.lynx.core.impl.RenderObjectImpl;
import com.lynx.ui.LynxUI;

import static com.lynx.core.impl.RenderObjectAttr.TEXT_LAYOUT;

public class LynxUILabel extends LynxUI<AndroidLabel> {

    public LynxUILabel(Context context, RenderObjectImpl impl) {
        super(context, impl);
    }

    @Override
    protected AndroidLabel createView(Context context) {
        return new AndroidLabel(context, this);
    }

    public Layout getLayout() {
        return (Layout) mRenderObjectImpl.getData(TEXT_LAYOUT.value());
    }

    @Override
    public void updateStyle(Style style) {
        super.updateStyle(style);
        if (style != null) {
            setPadding(style);
        }
    }

    @Override
    public void setData(int attr, Object param) {
        if(attr == TEXT_LAYOUT.value()) {
            mView.invalidate();
        }
        super.setData(attr, param);
    }

}
