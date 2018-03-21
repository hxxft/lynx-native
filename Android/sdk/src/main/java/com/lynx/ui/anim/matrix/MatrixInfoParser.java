// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.ui.anim.matrix;

import com.lynx.ui.anim.AnimInfo;

public class MatrixInfoParser {

    public static MatrixTweenInfo parser(AnimInfo from, AnimInfo to, int bw, int bh) {

        if (to == null) {
            return null;
        }

        MatrixTweenInfo info = new MatrixTweenInfo();

        fillBaseInfo(from, info, bw, bh);

        checkIfTranslate(to, info);
        checkIfRotate(to, info);
        checkIfScale(to, info);
        checkIfSkew(to, info);
        checkIfOpacity(to, info);
        checkIfPivot(to, info, bw, bh);

        info.offsetPerspective = to.perspective - info.basePerspective;

        if (from.option != null) {
            info.interpolator = from.option.interpolator;
        }

        return info;
    }

    private static void fillBaseInfo(AnimInfo from, MatrixTweenInfo info, int bw, int bh) {
        if (from == null) {
            return;
        }

        info.baseTranslateX = from.translateX;
        info.baseTranslateY = from.translateY;
        info.baseTranslateZ = from.translateZ;

        info.baseScaleX = from.scaleX;
        info.baseScaleY = from.scaleY;
        info.baseScaleZ = from.scaleZ;

        info.baseRotateX = from.rotateX;
        info.baseRotateY = from.rotateY;
        info.baseRotateZ = from.rotateZ;

        info.baseSkewX = from.skewX;
        info.baseSkewY = from.skewY;

        info.basePivotX = from.pivotX * bw;
        info.basePivotY = from.pivotY * bh;

        info.baseOpacity = opacityConstraint(from.opacity);
        info.basePerspective = from.perspective;
    }

    private static void checkIfRotate(AnimInfo object, MatrixTweenInfo info) {

        float angleX = object.rotateX;
        float angleY = object.rotateY;
        float angleZ = object.rotateZ;

        if (angleX != info.baseRotateX ||
                angleY != info.baseRotateY ||
                angleZ != info.baseRotateZ) {
            info.rotateEnable = true;

            info.offsetRotateX = angleX - info.baseRotateX;
            info.offsetRotateY = angleY - info.baseRotateY;
            info.offsetRotateZ = angleZ - info.baseRotateZ;
        }
    }

    private static void checkIfScale(AnimInfo object, MatrixTweenInfo info) {

        float scaleX = object.scaleX;
        float scaleY = object.scaleY;
        float scaleZ = object.scaleZ;

        if (scaleX != info.baseScaleX ||
                scaleY != info.baseScaleY ||
                scaleZ != info.baseScaleZ) {
            info.scaleEnable = true;
            info.offsetScaleX = scaleX - info.baseScaleX;
            info.offsetScaleY = scaleY - info.baseScaleY;
            info.offsetScaleZ = scaleZ - info.baseScaleZ;
        }
    }

    private static void checkIfTranslate(AnimInfo object, MatrixTweenInfo info) {
        float translateX = object.translateX;
        float translateY = object.translateY;
        float translateZ = object.translateZ;

        if (translateX != info.baseTranslateX ||
                translateY != info.baseTranslateY ||
                translateZ != info.baseTranslateZ) {
            info.translateEnable = true;
            info.offsetTranslateX = translateX - info.baseTranslateX;
            info.offsetTranslateY = translateY - info.baseTranslateY;
            info.offsetTranslateZ = translateZ - info.baseTranslateZ;
        }
    }

    private static void checkIfSkew(AnimInfo object, MatrixTweenInfo info) {
        float skewX = object.skewX;
        float skewY = object.skewY;

        if (skewX != info.baseSkewX ||
                skewY != info.baseSkewY) {
            info.skewEnable = true;
            info.offsetSkewX = skewX - info.baseSkewX;
            info.offsetSkewY = skewY - info.baseSkewY;
        }
    }

    private static void checkIfPivot(AnimInfo object, MatrixTweenInfo info, int bw, int bh) {
        float pivotX = object.pivotX * bw;
        float pivotY = object.pivotY * bh;

        if (pivotX != info.basePivotX ||
                pivotY != info.basePivotY) {
            info.pivotEnable = true;
            info.offsetPivotX = pivotX - info.basePivotX;
            info.offsetPivotY = pivotY - info.basePivotY;
        }
    }

    private static void checkIfOpacity(AnimInfo object, MatrixTweenInfo info) {
        float toOpacity = opacityConstraint(object.opacity);
        if (info.baseOpacity == toOpacity) {
            info.opacityEnable = false;
        } else {
            info.offsetOpacity = toOpacity - info.baseOpacity;
            info.opacityEnable = true;
        }
    }

    private static float opacityConstraint(float value) {
        if (value < 0.0f)
            value = 0;
        else if (value > 1.0f)
            value = 1;
        return value;
    }

}
