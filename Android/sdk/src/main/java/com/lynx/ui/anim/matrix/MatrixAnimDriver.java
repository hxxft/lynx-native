// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.ui.anim.matrix;

import android.text.TextUtils;
import android.view.View;
import android.view.animation.Animation;

import com.lynx.ui.LynxUI;
import com.lynx.ui.anim.AnimDriver;
import com.lynx.ui.anim.AnimInfo;

import java.util.List;

public class MatrixAnimDriver extends AnimDriver {
    private Animation mAnimation;
    private View mTarget;
    private float mOriginOpacity;
    private boolean mIsStop = false;
    // Prevent disorder of the Start and End callback while duration is zero.
    private boolean mIsStart = false;

    private MatrixAnimDriver(final LynxUI proxy) {
        mTarget = proxy.getView();
        saveRelevantStatus();
    }

    public MatrixAnimDriver(final LynxUI proxy,
                            List<AnimInfo> infoList,
                            final AnimInfo.Option option) {
        this(proxy);

        MatrixKeyFrames keyFrames = new MatrixKeyFrames();

        if (option.iterations != 0) {
            for (int i = 1; i < infoList.size(); i++) {
                AnimInfo first = infoList.get(i - 1);
                AnimInfo second = infoList.get(i);
                MatrixTweenInfo matrix3dInfo = MatrixInfoParser.parser(first, second,
                        mTarget.getWidth(), mTarget.getHeight());
                keyFrames.setKeyFrame(first.option.offset, matrix3dInfo);
            }

            keyFrames.setDuration(option.duration == 0 ? 1 : option.duration);
            keyFrames.setFillEnabled(true);
            keyFrames.setFillBefore(option.fillBefore);
            keyFrames.setFillAfter(option.fillAfter);
            keyFrames.setInterpolator(option.interpolator);
            keyFrames.setRepeatMode(option.repeatMode);
            keyFrames.setRepeatCount(option.iterations - 1);
            keyFrames.setDirection(option.direction);
        } else {
            // Avoids 0 in duration as Animation will ignore start offset.
            keyFrames.setDuration(1);
        }
        keyFrames.setStartOffset(option.delay);

        mAnimation = keyFrames;
        mAnimation.setAnimationListener(new Animation.AnimationListener() {
            @Override
            public void onAnimationStart(Animation animation) {
            }
            @Override
            public void onAnimationEnd(Animation animation) {
                if (mAnimation.hasEnded()) {
                    proxy.postEvent(AnimInfo.Option.EVENT_FINISH + option.id);
                }
            }
            @Override
            public void onAnimationRepeat(Animation animation) {}
        });

        mTarget.startAnimation(keyFrames);

    }

    @Override
    public void startAnim() {
        if (mAnimation != null) {
            mAnimation.start();
        }
    }

    @Override
    public void stopAnim() {
        mIsStop = true;
        if (mAnimation != null) {
            mAnimation.cancel();
        }
        restoreRelevantStatus();
    }

    private void saveRelevantStatus() {
        // Opacity Animation will not be fully enabled when view's alpha < 1 so that set
        // alpha to 1 to enable animation. Keeps origin value, Resets when stop.
        mOriginOpacity = mTarget.getAlpha();
        if (mOriginOpacity != 1) {
            mTarget.setAlpha(1);
        }
    }

    private void restoreRelevantStatus() {
        mTarget.setAlpha(mOriginOpacity);
    }
}