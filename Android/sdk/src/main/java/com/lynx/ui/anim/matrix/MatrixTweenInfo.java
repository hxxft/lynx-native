// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.ui.anim.matrix;


import android.view.animation.Interpolator;

public class MatrixTweenInfo {

    public float basePerspective;
    public float offsetPerspective;

    public float baseTranslateX;
    public float baseTranslateY;
    public float baseTranslateZ;
    public float baseRotateX;
    public float baseRotateY;
    public float baseRotateZ;
    public float baseScaleX = 1;
    public float baseScaleY = 1;
    public float baseScaleZ = 1;
    public float baseSkewX;
    public float baseSkewY;
    public float basePivotX;
    public float basePivotY;

    public boolean translateEnable;
    public float offsetTranslateX;
    public float offsetTranslateY;
    public float offsetTranslateZ;

    public boolean rotateEnable;
    public float offsetRotateX;
    public float offsetRotateY;
    public float offsetRotateZ;

    public boolean scaleEnable;
    public float offsetScaleX;
    public float offsetScaleY;
    public float offsetScaleZ;

    public boolean skewEnable;
    public float offsetSkewX;
    public float offsetSkewY;

    // Opacity change
    public boolean opacityEnable;
    public float baseOpacity = 1;
    public float offsetOpacity;

    public boolean pivotEnable;
    public float offsetPivotX;
    public float offsetPivotY;

    public Interpolator interpolator;
}
