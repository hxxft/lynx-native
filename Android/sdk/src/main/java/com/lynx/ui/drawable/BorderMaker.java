// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.ui.drawable;

import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.RectF;
import android.support.annotation.NonNull;

import com.lynx.base.Position;
import com.lynx.base.Style;

public class BorderMaker implements IMaker {
    private RectF mBorderRectF;
    private RectF mInnerBorderRectF;
    private Paint mBorderPaint;
    private Paint mInnerBorderPaint;

    private boolean needBorder = false;
    private boolean needInnerBorder = false;

    private float mBorderRadius = 0;
    private float mBorderWidth = 0;
    private float mWidth = 0;
    private float mHeight = 0;
    private float mEdge = 0;
    private float mHelperEdge = 0;

    public BorderMaker() {
        mBorderPaint = new Paint(Paint.ANTI_ALIAS_FLAG);
        mBorderRectF = new RectF();

        mInnerBorderPaint = new Paint(Paint.ANTI_ALIAS_FLAG);
        mInnerBorderRectF = new RectF();
    }

    @Override
    public void updateBounds(Position bounds) {
        mWidth = bounds.getWidth();
        mHeight = bounds.getHeight();
        syncRecF();
    }

    @Override
    public void updateStyle(Style style) {
        float borderRadius = (float) (style.mBorderRadius);
        // 设置border属性
        if ((float) style.mBorderWidth > 0) {
            needBorder = true;
            mBorderPaint.setColor(style.mBorderColor);
            mBorderPaint.setAntiAlias(true);
            mBorderPaint.setStrokeWidth((float) style.mBorderWidth);
            mBorderPaint.setStyle(Paint.Style.STROKE);
            mBorderWidth = (float) style.mBorderWidth;
            mEdge = mBorderWidth / 2;
            // 设置border的radius
            if (mBorderWidth > borderRadius) {
                // 当border width > radius时候，应该绘制两层，一层外圆角边框，一层内直角边框
                needInnerBorder = true;
                // 重新设置外边框绘制radius和border width
                mBorderRadius = borderRadius / 2;
                mBorderPaint.setStrokeWidth(borderRadius);
                mEdge = borderRadius / 2;
                // 设置内边框的画笔以及绘制区域
                mInnerBorderPaint.setColor(style.mBorderColor);
                mInnerBorderPaint.setAntiAlias(true);
                mInnerBorderPaint.setStrokeWidth(mBorderWidth - borderRadius);
                mInnerBorderPaint.setStyle(Paint.Style.STROKE);
                mHelperEdge = borderRadius + (mBorderWidth - borderRadius) / 2;
            } else {
                // 当border width < radius，绘制应圆角边框，radius应该进行缩小
                mBorderRadius = borderRadius - mBorderWidth / 2;
                needInnerBorder = false;
            }
        } else {
            needBorder = false;
            mBorderRadius = 0;
        }
        syncRecF();
    }

    @Override
    public void draw(@NonNull Canvas canvas) {
        if (needBorder && mWidth != 0 && mHeight != 0) {
            // 绘制具有radius的圆角边框
            canvas.drawRoundRect(mBorderRectF, mBorderRadius, mBorderRadius, mBorderPaint);

            // 如果radius < border width，绘制内边框
            if (needInnerBorder) {
                canvas.drawRect(mInnerBorderRectF, mInnerBorderPaint);
            }
        }
    }

    private void syncRecF() {
        mBorderRectF.set(mEdge, mEdge, mWidth - mEdge, mHeight - mEdge);
        mInnerBorderRectF.set(mHelperEdge, mHelperEdge, mWidth - mHelperEdge, mHeight - mHelperEdge);
    }

    private void reset() {
        mBorderPaint.reset();
        mInnerBorderPaint.reset();
        mBorderRectF.setEmpty();
        needBorder = false;
        needInnerBorder = false;
    }
}
