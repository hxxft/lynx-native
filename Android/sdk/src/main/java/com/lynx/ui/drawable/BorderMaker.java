// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.ui.drawable;

import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.RectF;
import android.support.annotation.NonNull;

import com.lynx.base.Position;
import com.lynx.base.Style;

public class BorderMaker implements IMaker {
    private IControl mControl;
    private RectF mBorderRectF;
    private RectF mInnerBorderRectF;
    private Paint mBorderPaint;
    private Paint mInnerBorderPaint;

    private boolean needBorder = false;
    private boolean needInnerBorder = false;

    private int mBorderColor = 0;
    private float mBorderRadius = 0;
    private float mBorderWidth = 0;
    private float mWidth = 0;
    private float mHeight = 0;
    private float mEdge = 0;
    private float mHelperEdge = 0;

    public BorderMaker(IControl control) {
        mControl = control;

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
        boolean requireInvalidate = false;
        float borderRadius = (float) (style.mBorderRadius);
        if (mBorderColor != style.mBorderColor) {
            mBorderColor = style.mBorderColor;
            mBorderPaint.setColor(mBorderColor);
            mInnerBorderPaint.setColor(mBorderColor);
            requireInvalidate = true;
        }
        if (mBorderWidth != style.mBorderWidth) {
            needBorder = true;
            mBorderWidth = (float) style.mBorderWidth;
            mBorderPaint.setAntiAlias(true);
            mBorderPaint.setStyle(Paint.Style.STROKE);
            mBorderPaint.setStrokeWidth(mBorderWidth);
            mEdge = mBorderWidth / 2;
            // Set border radius
            if (mBorderWidth > borderRadius) {
                // When border width > radius, two layer should be draw, an outer round border
                // and an inner right-angle border
                needInnerBorder = true;
                // Set radius and width of outer border
                mBorderRadius = borderRadius / 2;
                mBorderPaint.setStrokeWidth(borderRadius);
                mEdge = borderRadius / 2;
                // Set paint and rect of inner border
                mInnerBorderPaint.setAntiAlias(true);
                mInnerBorderPaint.setStyle(Paint.Style.STROKE);
                mInnerBorderPaint.setStrokeWidth(mBorderWidth - borderRadius);
                mHelperEdge = borderRadius + (mBorderWidth - borderRadius) / 2;
            } else {
                // When border width < radius，only draw a round border and shrink the radius
                mBorderRadius = borderRadius - mBorderWidth / 2;
                needInnerBorder = false;
            }
            requireInvalidate = true;
        }
        syncRecF();
        needBorder = mBorderWidth > 0 && mBorderColor != 0;
        requireInvalidate = requireInvalidate && needBorder;
        if (requireInvalidate) {
            mControl.invalidate();
        }
    }

    @Override
    public void draw(@NonNull Canvas canvas) {
        if (needBorder && mWidth != 0 && mHeight != 0) {
            // Draw outer round border
            canvas.drawRoundRect(mBorderRectF, mBorderRadius, mBorderRadius, mBorderPaint);

            // Draw inner right-angle border if needed
            if (needInnerBorder) {
                canvas.drawRect(mInnerBorderRectF, mInnerBorderPaint);
            }
        }
    }

    private void syncRecF() {
        mBorderRectF.set(mEdge, mEdge, mWidth - mEdge, mHeight - mEdge);
        mInnerBorderRectF.set(mHelperEdge, mHelperEdge, mWidth - mHelperEdge, mHeight - mHelperEdge);
    }

}
