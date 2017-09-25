// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.ui.anim.matrix;


public class AnimInformation {

    public float perspective;

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

    public boolean translateEnable;
    public float translateX;
    public float translateY;
    public float translateZ;

    public boolean rotateEnable;
    public float rotateX;
    public float rotateY;
    public float rotateZ;

    public boolean scaleEnable;
    public float scaleX;
    public float scaleY;
    public float scaleZ;

    public boolean skewEnable;
    public float skewX;
    public float skewY;

    // Opacity change
    public boolean opacityEnable;
    public float baseOpacity;
    public float opacity;
}
