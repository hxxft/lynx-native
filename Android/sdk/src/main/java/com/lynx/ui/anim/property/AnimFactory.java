// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.ui.anim.property;

import android.animation.Animator;
import android.animation.ObjectAnimator;
import android.animation.PropertyValuesHolder;
import android.view.View;
import android.view.animation.AccelerateDecelerateInterpolator;
import android.view.animation.AccelerateInterpolator;
import android.view.animation.DecelerateInterpolator;
import android.view.animation.Interpolator;
import android.view.animation.LinearInterpolator;

import com.lynx.ui.anim.AnimProperties;
import com.lynx.ui.anim.AnimTimingFunction;

public class AnimFactory {

    public static Animator createAnimator(View impl, AnimProperties properties) {
        if (properties.type == null) {
            return null;
        }
        // 创建速度控制器
        Interpolator interpolator = null;
        switch (AnimTimingFunction.is(properties.type)) {
            case LINEAR:
                interpolator = new LinearInterpolator(); break;
            case EASE_IN:
                interpolator = new AccelerateInterpolator(); break;
            case EASE_OUT:
                interpolator = new DecelerateInterpolator(); break;
            case EASE:
            case EASE_IN_OUT:
                interpolator = new AccelerateDecelerateInterpolator(); break;
            default: break;
        }
        // 创建对应属性动画
        PropertyValuesHolder holderAlpha = PropertyValuesHolder.ofFloat("alpha",
                properties.fromOpacity, properties.toOpacity);
        PropertyValuesHolder holderScaleX = PropertyValuesHolder.ofFloat("scaleX",
                properties.fromXScale, properties.toXScale);
        PropertyValuesHolder holderScaleY = PropertyValuesHolder.ofFloat("scaleY",
                properties.fromYScale, properties.toYScale);

        // Transform the sp size to pixel
        // TODO 需要将底层传过来的sp转化成px
        properties.fromLeft = (float) Math.ceil(properties.fromLeft);
        properties.toLeft = (float) Math.ceil(properties.toLeft);
        properties.fromTop = (float) Math.ceil(properties.fromTop);
        properties.toTop = (float) Math.ceil(properties.toTop);

        PropertyValuesHolder holderTranslationX = PropertyValuesHolder.ofFloat("translationX",
                properties.fromLeft, properties.toLeft);
        PropertyValuesHolder holderTranslationY = PropertyValuesHolder.ofFloat("translationY",
                properties.fromTop, properties.toTop);

        Animator animator = ObjectAnimator.ofPropertyValuesHolder(impl, holderAlpha, holderScaleX,
                holderScaleY, holderTranslationX, holderTranslationY);
        animator.setDuration(properties.time);
        animator.setInterpolator(interpolator);
        return animator;
    }
}
