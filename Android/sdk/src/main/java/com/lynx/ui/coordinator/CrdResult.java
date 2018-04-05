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
    private float mOriginY = 0;
    private float mOriginX = 0;
    private float mOpacity = 0;
    private float mOffsetTop = 0;
    private float mOffsetBottom = 0;
    private float mOffsetLeft = 0;
    private float mOffsetRight = 0;
    private boolean mIsConsumed = false;
    private long mDuration = 0;
    private int mTimingFunction = 0;
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
        mOriginX = convertToPx(dResult[i++]);
        mOriginY = convertToPx(dResult[i++]);
        mOpacity = (float) dResult[i++];
        mOffsetTop = convertToPx(dResult[i++]);
        mOffsetLeft = convertToPx(dResult[i++]);
        mOffsetBottom = convertToPx(dResult[i++]);
        mOffsetRight = convertToPx(dResult[i++]);
        mIsConsumed = dResult[i++] != 0;
        mDuration = (long) dResult[i++];
        mTimingFunction = (int) dResult[i++];

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
        return mOriginX;
    }

    public float getPivotY() {
        return mOriginY;
    }

    public float getAlpha() {
        return mOpacity;
    }

    public float getBottomOffset() {
        return mOffsetBottom;
    }

    public float getTopOffset() {
        return mOffsetTop;
    }

    public float getRightOffset() {
        return mOffsetRight;
    }

    public float getLeftOffset() {
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
        return mTimingFunction;
    }

}
