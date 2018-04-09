// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.ui.drawable;

import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.RectF;
import android.support.annotation.NonNull;

import com.lynx.base.Position;
import com.lynx.base.Style;

public class BgColorMaker implements IMaker{
    private IControl mControl;
    private RectF mBackgroundRectF;
    private Paint mBackgroundPaint;

    private boolean needBgColor = true;

    private float mBackgroundRadius = 0;
    private float mWidth = 0;
    private float mHeight = 0;
    private int mCurrentColor = 0;

    public BgColorMaker(IControl control) {
        mControl = control;
        mBackgroundPaint = new Paint(Paint.ANTI_ALIAS_FLAG);
        mBackgroundRectF = new RectF();
    }

    private void syncRecF() {
        mBackgroundRectF.set(0, 0, mWidth, mHeight);
    }

    @Override
    public void updateBounds(@NonNull Position bounds) {
        mWidth = bounds.getWidth();
        mHeight = bounds.getHeight();
        syncRecF();
    }

    @Override
    public void updateStyle(@NonNull Style style) {
        boolean requireInvalidate = false;
        if (mCurrentColor != style.mBackgroundColor) {
            mBackgroundPaint.setColor(style.mBackgroundColor);
            mCurrentColor = style.mBackgroundColor;
            requireInvalidate = true;
        }
        needBgColor = mCurrentColor != 0;
        if (mBackgroundRadius != style.mBorderRadius) {
            mBackgroundRadius = style.mBorderRadius > 0 ? (float) (style.mBorderRadius) : 0;
            requireInvalidate = true;
        }
        syncRecF();
        requireInvalidate = requireInvalidate && needBgColor && mWidth != 0 && mHeight != 0;
        if (requireInvalidate) {
            mControl.invalidate();
        }
    }

    @Override
    public void draw(@NonNull Canvas canvas) {
        if (needBgColor && mWidth != 0 && mHeight != 0) {
            // 绘制背景
            canvas.drawRoundRect(mBackgroundRectF, mBackgroundRadius, mBackgroundRadius, mBackgroundPaint);
        }
    }

}
