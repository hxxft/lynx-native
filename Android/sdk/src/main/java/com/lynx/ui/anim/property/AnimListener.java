// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.ui.anim.property;

import android.animation.Animator;

import com.lynx.core.impl.RenderObjectImpl;

public class AnimListener implements Animator.AnimatorListener {
    private RenderObjectImpl mRenderObjectImpl;
    private String mEvent;
    private boolean mIsStop = false;

    public AnimListener(RenderObjectImpl impl, String animEvent) {
        mRenderObjectImpl = impl;
        mEvent = animEvent;
    }

    @Override
    public void onAnimationStart(Animator animation) {
        Object[] objects = new Object[1];
        objects[0] = true;
        mRenderObjectImpl.dispatchEvent(mEvent, objects);
    }

    @Override
    public void onAnimationEnd(Animator animation) {
        if (!mIsStop) {
            Object[] objects = new Object[1];
            objects[0] = false;
            mRenderObjectImpl.dispatchEvent(mEvent, objects);
        }
    }

    @Override
    public void onAnimationCancel(Animator animation) {

    }

    @Override
    public void onAnimationRepeat(Animator animation) {

    }

    public void stop() {
        mIsStop = true;
    }
}
