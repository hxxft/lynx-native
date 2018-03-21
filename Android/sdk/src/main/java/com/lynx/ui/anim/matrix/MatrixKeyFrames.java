// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.ui.anim.matrix;

import android.view.animation.Interpolator;
import android.view.animation.Transformation;

import com.lynx.ui.anim.AnimInfo;

import java.util.ArrayList;
import java.util.List;

public class MatrixKeyFrames extends MatrixAnimation {
    private final static KeyFrameFinder sReverseFinder = new ReverseFinder();
    private final static KeyFrameFinder sSequenceFinder = new SequenceFinder();

    private int mCurIndex;
    private int mDirection;
    private List<Pair> mKeyFrames;
    private float mPreInterpolatedTime;

    public MatrixKeyFrames() {
        super();
        mKeyFrames = new ArrayList<>();
        mDirection = AnimInfo.Option.FORWARDS;
        mCurIndex = -1;
    }

    public void setKeyFrame(float offset, MatrixTweenInfo info) {
        mKeyFrames.add(new Pair(offset, info));
    }

    @Override
    public void setRepeatMode(int repeatMode) {
        super.setRepeatMode(repeatMode);
    }

    public void setDirection(int direction) {
        mDirection = direction;
    }

    @Override
    public boolean getTransformation(long currentTime, Transformation outTransformation) {
        // While start the repeat action, reset the mCurIndex. Meanwhile reset start offset as
        // start offset only works on the first cycle in web standard.
        if (getStartTime() == -1 && mCurIndex != -1) {
            mCurIndex = -1;
            setStartOffset(0);
        }
        return super.getTransformation(currentTime, outTransformation);
    }

    @Override
    protected void applyTransformation(float interpolatedTime, Transformation t) {
        // When fillBefore set to true, Animation.initializeInvalidateRegion() will be invoke when
        // Animation init, and the start offset will not working.
        if (!hasStarted() && getStartOffset() != 0) return;
        if (mDirection != AnimInfo.Option.FORWARDS) {
            interpolatedTime = 1 - interpolatedTime;
        }
        if (!mKeyFrames.isEmpty()) {
            KeyFrameFinder finder = interpolatedTime - mPreInterpolatedTime > 0 ?
                    sSequenceFinder : sReverseFinder;
            mCurIndex = finder.findCurKeyFrame(mCurIndex, interpolatedTime, mKeyFrames);
            setInfo(mKeyFrames.get(mCurIndex).info);
            super.applyTransformation(
                    finder.getNormalizedTime(mCurIndex, interpolatedTime, mKeyFrames), t);
            mPreInterpolatedTime = interpolatedTime;
        }
    }

    private static class Pair {
        float offset;
        MatrixTweenInfo info;

        Pair(float offset, MatrixTweenInfo info) {
            this.offset = offset;
            this.info = info;
        }
    }

    // Gets tween info in animation cycle
    private interface KeyFrameFinder {
        // Gets the index of current tween info
        int findCurKeyFrame(int curIndex, float interpolatedTime, List<Pair> keyFrames);
        // Gets normalized time between current info and next info
        float getNormalizedTime(int curIndex, float interpolatedTime, List<Pair> keyFrames);
    }

    private static class SequenceFinder implements KeyFrameFinder {

        @Override
        public int findCurKeyFrame(int curIndex, float interpolatedTime, List<Pair> keyFrames) {
            if (curIndex < 0) curIndex = 0;
            while (curIndex < keyFrames.size() - 1 && curIndex >= 0
                    && interpolatedTime > keyFrames.get(curIndex + 1).offset) {
                ++curIndex;
            }
            return curIndex;
        }

        @Override
        public float getNormalizedTime(int curIndex, float interpolatedTime, List<Pair> keyFrames) {
            float curOffset = keyFrames.get(curIndex).offset;
            float nextOffset = curIndex < keyFrames.size() - 1 ?
                    keyFrames.get(curIndex + 1).offset : 1;
            float normalizedTime = (interpolatedTime - curOffset) / (nextOffset - curOffset);
            Interpolator interpolator = keyFrames.get(curIndex).info.interpolator;
            if (interpolator != null) {
                normalizedTime = interpolator.getInterpolation(normalizedTime);
            }
            return normalizedTime;
        }
    }

    private static class ReverseFinder implements KeyFrameFinder {

        @Override
        public int findCurKeyFrame(int curIndex, float interpolatedTime, List<Pair> keyFrames) {
            if (curIndex < 0) curIndex = keyFrames.size() - 1;
            while (curIndex < keyFrames.size() && curIndex >= 1
                    && interpolatedTime < keyFrames.get(curIndex).offset) {
                --curIndex;
            }
            return curIndex;
        }

        public float getNormalizedTime(int curIndex, float interpolatedTime, List<Pair> keyFrames) {
            float curOffset = keyFrames.get(curIndex).offset;
            float preOffset = curIndex < keyFrames.size() - 1 ?
                    keyFrames.get(curIndex + 1).offset : 1;
            float normalizedTime = (interpolatedTime - curOffset) / (preOffset - curOffset);
            Interpolator interpolator = keyFrames.get(curIndex).info.interpolator;
            if (interpolator != null) {
                normalizedTime = interpolator.getInterpolation(normalizedTime);
            }
            return normalizedTime;
        }
    }
}
