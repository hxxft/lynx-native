// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.ui.anim.property;

import android.animation.Animator;

import com.lynx.ui.LynxUI;
import com.lynx.ui.anim.AnimDriver;
import com.lynx.ui.anim.AnimProperties;

public class PropertyAnimDriver extends AnimDriver {

    private AnimProperties mOriginProperties;
    private Animator mAnimator;
    private LynxUI mUI;
    private AnimListener mListener;

    public PropertyAnimDriver(LynxUI ui, String animEvent,
                              AnimProperties properties) {
        mUI = ui;
        // 根据参数创建对应的animator
        mAnimator = AnimFactory.createAnimator(ui.getView(), properties);
        // 将event加入EventSet中，以便函数回调可以通过shadowNode到BaseNode找到对应的jsFunction
        if (animEvent != null) {
            mListener = new AnimListener(ui.getRenderObjectImpl(), animEvent);
            mAnimator.addListener(mListener);
        }
        if (mOriginProperties == null) {
            mOriginProperties = properties;
        }
    }

    @Override
    public void startAnim() {
        mAnimator.start();
    }

    @Override
    public void stopAnim() {
        if (mAnimator.isRunning()) {
            if (mListener != null) {
                mListener.stop();
            }
            mAnimator.cancel();
        }
        // reset to origin
        mUI.getView().setScaleX(1);
        mUI.getView().setScaleY(1);
        mUI.getView().setTranslationX(0);
        mUI.getView().setTranslationY(0);
        mUI.getView().setAlpha(1);
    }

}
