// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.ui.coordinator;

import com.lynx.utils.PixelUtil;

/* package */ class CrdResult {
    final static int NOT_SET = Integer.MAX_VALUE;
    private float mTranslateX = 0;
    private float mTranslateY = 0;
    private float mScaleX = 0;
    private float mScaleY = 0;
    private float mRotationY = 0;
    private float mRotationX = 0;
    private float mPivotY = 0;
    private float mPivotX = 0;
    private float mAlpha = 0;
    private float mOffsetTop = 0;
    private float mOffsetBottom = 0;
    private float mOffsetLeft = 0;
    private float mOffsetRight = 0;
    private boolean mIsConsumed = false;
    private long mDuration = 0;
    private int mInterpolatorType = 0;
    private String mEvent;
    private Object mParamsForEvent = 0;

    public CrdResult(Object[] result) {
        // parse action
        double[] dResult = (double[]) result[0];
        int i = 0;
        mTranslateX = convertToPx(dResult[i++]);
        mTranslateY = convertToPx(dResult[i++]);
        mScaleX = (float) dResult[i++];
        mScaleY = (float) dResult[i++];
        mRotationX = (float) dResult[i++];
        mRotationY = (float) dResult[i++];
        mPivotX = convertToPx(dResult[i++]);
        mPivotY = convertToPx(dResult[i++]);
        mAlpha = (float) dResult[i++];
        mOffsetTop = convertToPx(dResult[i++]);
        mOffsetLeft = convertToPx(dResult[i++]);
        mOffsetBottom = convertToPx(dResult[i++]);
        mOffsetRight = convertToPx(dResult[i++]);
        mIsConsumed = dResult[i++] != 0;
        mDuration = (long) dResult[i++];
        mInterpolatorType = (int) dResult[i++];

        // parse event
        Object[] eResult = (Object[]) result[1];
        if (eResult != null) {
            mEvent = (String) eResult[0];
            mParamsForEvent = eResult[1];
        }
    }

    protected float convertToPx(double number) {
        if (number == NOT_SET) {
            return NOT_SET;
        }
        return (float) PixelUtil.lynxNumberToPx(number);
    }

    public float getTranslateX() {
        return mTranslateX;
    }

    public float getTranslateY() {
        return mTranslateY;
    }

    public float getScaleX() {
        return mScaleX;
    }

    public float getScaleY() {
        return mScaleY;
    }

    public float getRotationY() {
        return mRotationY;
    }

    public float getRotationX() {
        return mRotationX;
    }

    public float getPivotX() {
        return mPivotX;
    }

    public float getPivotY() {
        return mPivotY;
    }

    public float getAlpha() {
        return mAlpha;
    }

    public float getOffsetBottom() {
        return mOffsetBottom;
    }

    public float getOffsetTop() {
        return mOffsetTop;
    }

    public float getOffsetRight() {
        return mOffsetRight;
    }

    public float getOffsetLeft() {
        return mOffsetLeft;
    }

    public boolean isConsumed() {
        return mIsConsumed;
    }

    public long getDuration() {
        return mDuration;
    }

    public String getEvent() {
        return mEvent;
    }

    // Note: now only basic data types and String
    public Object getParamsForEvent() {
        return mParamsForEvent;
    }

    public int getInterpolatorType() {
        return mInterpolatorType;
    }

}
