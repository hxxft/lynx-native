// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.ui.anim.matrix;

import android.text.TextUtils;
import android.view.View;
import android.view.animation.AccelerateDecelerateInterpolator;
import android.view.animation.AccelerateInterpolator;
import android.view.animation.Animation;
import android.view.animation.DecelerateInterpolator;
import android.view.animation.LinearInterpolator;

import com.lynx.ui.LynxUI;
import com.lynx.ui.anim.AnimDriver;
import com.lynx.ui.anim.AnimProperties;
import com.lynx.ui.anim.AnimTimingFunction;

public class MatrixAnimDriver extends AnimDriver {

    private Matrix3dAnimation mAnimation;
    private AnimInformation mInfo;
    private View mTarget;
    private float mOriginOpacity;
    private boolean mIsStop = false;
    // Prevent disorder of the Start and End callback while duration is zero.
    private boolean mIsStart = false;

    public MatrixAnimDriver(final LynxUI ui, final String animEvent,
                            AnimProperties from, AnimProperties to) {

        // Get AnimInformation
        if (from != null) {
            mInfo = Matrix3dParser.parser(from.toMatrix3d, to.toMatrix3d,
                    to.fromOpacity, to.toOpacity, from.perspective);
        } else {
            mInfo = Matrix3dParser.parser(null, to.toMatrix3d, to.fromOpacity,
                    to.toOpacity, to.perspective);
        }

        mTarget = ui.getView();

        float originX = XCast.toFloat(to.transformOrigin.get(0), 0) *
                ui.getView().getWidth();
        float originY = XCast.toFloat(to.transformOrigin.get(1), 0) *
                ui.getView().getHeight();

        mAnimation = new Matrix3dAnimation(mInfo, originX, originY);
        mAnimation.setDuration(to.time);
        mAnimation.setFillAfter(true);
        mAnimation.setFillEnabled(true);

        // Set interpolator
        switch (AnimTimingFunction.is(to.type)) {
            case LINEAR:
                mAnimation.setInterpolator(new LinearInterpolator()); break;
            case EASE_IN:
                mAnimation.setInterpolator(new AccelerateInterpolator()); break;
            case EASE_OUT:
                mAnimation.setInterpolator(new DecelerateInterpolator()); break;
            case EASE:
            case EASE_IN_OUT:
                mAnimation.setInterpolator(new AccelerateDecelerateInterpolator()); break;
            default: break;
        }

        // Set Listener
        mAnimation.setAnimationListener(new Animation.AnimationListener() {
            @Override
            public void onAnimationStart(Animation animation) {

                if (mIsStart) {
                    return;
                }

                if (!TextUtils.isEmpty(animEvent)) {
                    ui.postEvent(animEvent, true);
                }
                // Opacity Animation enable when view's alpha > 0 so that set alpha to
                // enable animation. And reset origin value when stop.
                if (mInfo.opacityEnable) {
                    mOriginOpacity = mTarget.getAlpha();
                    mTarget.setAlpha(1);
                }

                mIsStart = true;
            }

            @Override
            public void onAnimationEnd(Animation animation) {
                if (!mIsStart) {
                    onAnimationStart(animation);
                }
                if (!mIsStop && !TextUtils.isEmpty(animEvent)) {
                    ui.postEvent(animEvent, false);
                }
            }

            @Override
            public void onAnimationRepeat(Animation animation) {}
        });

        mTarget.startAnimation(mAnimation);
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
        if (mInfo != null && mInfo.opacityEnable) {
            mTarget.setAlpha(mOriginOpacity);
        }
    }
}