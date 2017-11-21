// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.ui.body;

import android.content.Context;
import android.view.MotionEvent;

import com.lynx.core.touch.TouchDispatcher;
import com.lynx.ui.view.AndroidViewGroup;

public class AndroidBody extends AndroidViewGroup {
    private TouchDispatcher mTouchDispatcher;

    public AndroidBody(Context context) {
        super(context, null);
        mUIGroup = new LynxUIBody(context);
        ((LynxUIBody) mUIGroup).bindView(this);
        mTouchDispatcher = new TouchDispatcher(context, mUIGroup);
    }

    @Override
    protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
        setMeasuredDimension(
                MeasureSpec.getSize(widthMeasureSpec),
                MeasureSpec.getSize(heightMeasureSpec));
        mUIGroup.measure();
    }

    @Override
    protected void onLayout(boolean changed, int l, int t, int r, int b) {
        super.onLayout(changed, l, t, r, b);
        mUIGroup.layout();
    }

    @Override
    public boolean dispatchTouchEvent(MotionEvent ev) {
        mTouchDispatcher.dispatchMotionEvent(ev);
        super.dispatchTouchEvent(ev);
        // Always return true so that receiving touch event successively
        return true;
    }

}
