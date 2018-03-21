// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.ui.anim;

import android.support.v4.view.animation.PathInterpolatorCompat;
import android.view.animation.AccelerateDecelerateInterpolator;
import android.view.animation.AccelerateInterpolator;
import android.view.animation.DecelerateInterpolator;
import android.view.animation.Interpolator;
import android.view.animation.LinearInterpolator;
import android.view.animation.PathInterpolator;

public class TimingFunction {

    public final static String EASING_LINEAR = "linear";
    public final static String EASING_EASE = "ease";
    public final static String EASING_EASE_IN = "ease-in";
    public final static String EASING_EASE_OUT = "ease-out";
    public final static String EASING_EASE_IN_OUT = "ease-in-out";
    public final static String EASING_CUBIC_BEZIER_PREFIX = "cubic-bezier";

    public static Interpolator transform(String easing) {
        Interpolator interpolator = null;
        switch (easing) {
            case EASING_LINEAR:
                interpolator = new LinearInterpolator(); break;
            case EASING_EASE_OUT:
                interpolator = new DecelerateInterpolator(); break;
            case EASING_EASE_IN:
                interpolator = new AccelerateInterpolator(); break;
            case EASING_EASE:
            case EASING_EASE_IN_OUT:
                interpolator = new AccelerateDecelerateInterpolator(); break;
            default:
                if (easing.startsWith(EASING_CUBIC_BEZIER_PREFIX)) {
                    float[] params = new float[4];
                    int index = 0;
                    StringBuilder temp = new StringBuilder();
                    for (int i = EASING_CUBIC_BEZIER_PREFIX.length() + 1; i < easing.length() - 1; i++) {
                        char c = easing.charAt(i);
                        if (Character.isDigit(c) || c == '.' || c == '-' || c == '+') {
                            temp.append(c);
                        } else if (c == ',') {
                            params[index++] = Float.valueOf(temp.toString());
                            temp = new StringBuilder();
                        }
                    }
                    params[index] = Float.valueOf(temp.toString());
                    interpolator = PathInterpolatorCompat.create(params[0], params[1],
                            params[2], params[3]);
                } else {
                    interpolator = new LinearInterpolator();
                }
                break;
        }
        return interpolator;
    }
}
