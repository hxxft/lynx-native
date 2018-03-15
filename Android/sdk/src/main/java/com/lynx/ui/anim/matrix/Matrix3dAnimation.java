// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.ui.anim.matrix;

import android.graphics.Camera;
import android.graphics.Matrix;
import android.view.animation.Animation;
import android.view.animation.Transformation;

public class Matrix3dAnimation extends Animation {

    private static final double DEGREE_TO_ROD = Math.PI / 180;

    private Camera mCamera;
    private Matrix mCurMatrix;
    private AnimInformation mInfo;
    private float mPivotX;
    private float mPivotY;

    public Matrix3dAnimation(AnimInformation info, float originX, float originY) {
        mInfo = info;
        mPivotX = originX;
        mPivotY = originY;
        mCamera = new Camera();
    }

    @Override
    protected void applyTransformation(float interpolatedTime, Transformation t) {
        if (mInfo == null) {
            return;
        }

        final Camera camera = mCamera;
        final Matrix matrix = t.getMatrix();

        camera.save();
        camera.setLocation(0, 0, mInfo.perspective);

        applyTranslateTransformationIfEnable(camera, interpolatedTime);
        applyRotateTransformationIfEnable(camera, interpolatedTime);

        camera.getMatrix(matrix);
        camera.restore();

        applyScaleTransformationIfEnable(matrix, interpolatedTime);
        applySkewTransformationIfEnable(matrix, interpolatedTime);

        // Translate to the target origin point
        matrix.preTranslate(-mPivotX, -mPivotY);
        matrix.postTranslate(mPivotX, mPivotY);

        applyOpacityTransformation(t, interpolatedTime);

        mCurMatrix = matrix;
    }

    private void applyTranslateTransformationIfEnable(Camera camera, float interpolatedTime) {

        float x, y, z;
        x = mInfo.baseTranslateX;
        y = mInfo.baseTranslateY;
        z = mInfo.baseTranslateZ;

        if (mInfo.translateEnable) {
            x += interpolatedTime * mInfo.translateX;
            y += interpolatedTime * mInfo.translateY;
            z += interpolatedTime * mInfo.translateZ;
        }
        camera.translate(x, -y, z);

    }

    private void applyRotateTransformationIfEnable(Camera camera, float interpolatedTime) {

        float x, y, z;

        x = mInfo.baseRotateX;
        y = mInfo.baseRotateY;
        z = mInfo.baseRotateZ;

        if (mInfo.rotateEnable) {
            x += interpolatedTime * mInfo.rotateX;
            y += interpolatedTime * mInfo.rotateY;
            z += interpolatedTime * mInfo.rotateZ;
        }
        // For keeping the same rotate direction between web and android, z and y is negative
        camera.rotate(x, -y, -z);
    }

    private void applyScaleTransformationIfEnable(Matrix matrix, float interpolatedTime) {

        float x, y;

        x = mInfo.baseScaleX;
        y = mInfo.baseScaleY;

        if (mInfo.scaleEnable) {
            x += interpolatedTime * mInfo.scaleX;
            y += interpolatedTime * mInfo.scaleY;
        }
        matrix.preScale(x, y);
    }

    private void applySkewTransformationIfEnable(Matrix matrix, float interpolatedTime) {
        float x, y;

        x = mInfo.baseSkewX;
        y = mInfo.baseSkewY;

        if (mInfo.skewEnable) {
            x += interpolatedTime * mInfo.skewX;
            y += interpolatedTime * mInfo.skewY;
        }
        matrix.preSkew((float) Math.tan(x * DEGREE_TO_ROD), (float) Math.tan(y * DEGREE_TO_ROD));
    }

    private void applyOpacityTransformation(Transformation t, float interpolatedTime) {
        if (mInfo.opacityEnable) {
            t.setAlpha(mInfo.baseOpacity + interpolatedTime * mInfo.opacity);
        }
    }

    public Matrix getMatrix() {
        return mCurMatrix;
    }
}
