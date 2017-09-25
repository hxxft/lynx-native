// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.ui.drawable;

import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.RectF;
import android.support.annotation.NonNull;

import com.lynx.base.Position;
import com.lynx.base.Style;

// TODO: 17/8/7 需要去除border占据的部分
public class BackgroundColorMaker implements IMaker{
    private RectF mBackgroundRectF;
    private Paint mBackgroundPaint;

    private boolean needBgColor = true;

    private float mBackgroundRadius = 0;
    private float mWidth = 0;
    private float mHeight = 0;
    private int mCurrentColor = 0;

    public BackgroundColorMaker() {
        mBackgroundPaint = new Paint(Paint.ANTI_ALIAS_FLAG);
        mBackgroundRectF = new RectF();
    }

    private void syncRecF() {
        mBackgroundRectF.set(0, 0, mWidth, mHeight);
    }

    public void reset() {
        mBackgroundPaint.reset();
        mBackgroundRectF.setEmpty();
        needBgColor = false;
    }

    @Override
    public void updateBounds(@NonNull  Position bounds) {
        mWidth = bounds.getWidth();
        mHeight = bounds.getHeight();
        syncRecF();
    }

    @Override
    public void updateStyle(@NonNull Style style) {
        if (style.mBackgroundColor != 0) {
            needBgColor = true;
            if (mCurrentColor != style.mBackgroundColor) {
                // 设置背景颜色
                mBackgroundPaint.setColor(style.mBackgroundColor);
                mCurrentColor = style.mBackgroundColor;
            }
        } else {
            needBgColor = false;
        }
        // 设置圆角
        if ((float) style.mBorderRadius > 0) {
            mBackgroundRadius = (float) (style.mBorderRadius);
        } else {
            mBackgroundRadius = 0;
        }
        syncRecF();
    }

    @Override
    public void draw(@NonNull Canvas canvas) {
        if (needBgColor && mWidth != 0 && mHeight != 0) {
            // 绘制背景
            canvas.drawRoundRect(mBackgroundRectF, mBackgroundRadius, mBackgroundRadius, mBackgroundPaint);
        }
    }
}
