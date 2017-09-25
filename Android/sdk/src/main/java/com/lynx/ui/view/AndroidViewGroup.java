// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.ui.view;

import android.content.Context;
import android.graphics.Canvas;
import android.view.ViewGroup;

import com.lynx.ui.LynxUIGroup;
import com.lynx.ui.utils.RoundRectHelper;

public class AndroidViewGroup extends ViewGroup {

    protected LynxUIGroup mUIGroup;

    public AndroidViewGroup(Context context, LynxUIGroup presenter) {
        super(context);
        mUIGroup = presenter;
    }

    @Override
    protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
        // Only setMeasuredDimension as {@link LynxUIGroup} handles actually
        // measuring out children.
        setMeasuredDimension(
                MeasureSpec.getSize(widthMeasureSpec),
                MeasureSpec.getSize(heightMeasureSpec));
    }

    @Override
    protected void onLayout(boolean changed,
                            int l, int t, int r, int b) {
        // No-op since {@link LynxUIGroup} handles actually laying out children.
    }

    @Override
    public void draw(Canvas canvas) {
        if (mUIGroup.getRenderObjectImpl() != null
                && mUIGroup.getRenderObjectImpl().getStyle() != null
                && mUIGroup.getRenderObjectImpl().getStyle().mBorderRadius > 0) {
            // 进行孩子的裁剪
            int count = canvas.saveLayer(
                    0,
                    0,
                    mUIGroup.getRenderObjectImpl().getPosition().getWidth(),
                    mUIGroup.getRenderObjectImpl().getPosition().getHeight(),
                    null,
                    Canvas.HAS_ALPHA_LAYER_SAVE_FLAG);
            super.draw(canvas);
            RoundRectHelper.instance()
                    .clipRoundRectWithXfermode(canvas, mUIGroup.getRenderObjectImpl());
            canvas.restoreToCount(count);
        } else {
            // 不裁剪孩子
            super.draw(canvas);
        }
    }

}
