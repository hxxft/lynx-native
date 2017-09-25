// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.ui.label;

import android.content.Context;
import android.graphics.Canvas;
import android.text.Layout;
import android.util.Log;
import android.view.View;

public class AndroidLabel extends View {

    private LynxUILabel mUI;

    public AndroidLabel(Context context, LynxUILabel presenter) {
        super(context);
        mUI = presenter;
    }

    @Override
    protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
        setMeasuredDimension(
                MeasureSpec.getSize(widthMeasureSpec),
                MeasureSpec.getSize(heightMeasureSpec));
    }

    @Override
    protected void onLayout(boolean changed,
                            int l, int t, int r, int b) {
    }

    @Override
    protected void onDraw(Canvas canvas) {
        Layout layout = mUI.getLayout();
        if (layout != null) {
            canvas.save();
            canvas.translate(getPaddingLeft(), getPaddingTop());
            layout.draw(canvas);
            canvas.restore();
        }
    }

}
