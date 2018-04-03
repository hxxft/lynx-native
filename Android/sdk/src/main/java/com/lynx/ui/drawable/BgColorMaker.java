// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.ui.drawable;

import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.RectF;
import android.support.annotation.NonNull;

import com.lynx.base.Position;
import com.lynx.base.Style;

public class BgColorMaker implements IMaker{
    private RectF mBackgroundRectF;
    private Paint mBackgroundPaint;

    private boolean needBgColor = true;

    private float mBackgroundRadius = 0;
    private float mWidth = 0;
    private float mHeight = 0;
    private float mWidthOffset = 0;
    private float mHeightOffset = 0;
    private int mCurrentColor = 0;

    public BgColorMaker() {
        mBackgroundPaint = new Paint(Paint.ANTI_ALIAS_FLAG);
        mBackgroundRectF = new RectF();
    }

    private void syncRecF() {
        mBackgroundRectF.set(0, 0, mWidth + mWidthOffset, mHeight + mHeightOffset);
    }

    @Override
    public void updateBounds(@NonNull Position bounds) {
        mWidth = bounds.getWidth();
        mHeight = bounds.getHeight();
        syncRecF();
    }

    @Override
    public void updateStyle(@NonNull Style style) {
        if (style.mBackgroundColor != 0) {
            needBgColor = true;
            if (mCurrentColor != style.mBackgroundColor) {
                mBackgroundPaint.setColor(style.mBackgroundColor);
                mCurrentColor = style.mBackgroundColor;
            }
        } else {
            needBgColor = false;
        }
        if (style.mBorderWidth != 0) {
            mHeightOffset = mWidthOffset = (float) (-style.mBorderWidth * 2);
        }
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
