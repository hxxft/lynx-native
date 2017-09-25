// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.ui.radio;

import android.content.Context;
import android.widget.RadioGroup;

public class AndroidRadioGroup extends RadioGroup {

    public AndroidRadioGroup(Context context) {
        super(context);
    }

    @Override
    protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
        setMeasuredDimension(
                MeasureSpec.getSize(widthMeasureSpec),
                MeasureSpec.getSize(heightMeasureSpec));
    }

    @Override
    protected void onLayout(boolean changed, int l, int t, int r, int b) {
        // No-op
    }
}
