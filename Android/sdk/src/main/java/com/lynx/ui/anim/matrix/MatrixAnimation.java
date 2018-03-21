// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.ui.anim.matrix;

import android.graphics.Camera;
import android.graphics.Matrix;
import android.view.animation.Animation;
import android.view.animation.Transformation;

public class MatrixAnimation extends Animation {
    private static final double DEGREE_TO_ROD = Math.PI / 180;

    private Camera mCamera;
    private MatrixTweenInfo mInfo;

    private Matrix mCurMatrix;

    public MatrixAnimation() {
        mCamera = new Camera();
    }

    public MatrixAnimation(MatrixTweenInfo info) {
        this();
        mInfo = info;
    }

    public void setInfo(MatrixTweenInfo info) {
        mInfo = info;
    }

    @Override
    protected void applyTransformation(float interpolatedTime, Transformation t) {
        if (mInfo == null) {
            return;
        }

        final Camera camera = mCamera;
        final Matrix matrix = t.getMatrix();

        camera.save();
        float perspective = mInfo.basePerspective + interpolatedTime * mInfo.offsetPerspective;
        // As the unit of setLocation() is inch and the value for conversion is 72
        // pixel written by hardcode in skia, makes a conversion in params.
        // Meanwhile it's not allowed to use 0 on the z axis, uses -8 as default instead.
        camera.setLocation(0, 0, perspective == 0? -8 : perspective / 72);

        applyTranslateTransformationIfEnable(camera, interpolatedTime);
        applyRotateTransformationIfEnable(camera, interpolatedTime);

        camera.getMatrix(matrix);
        camera.restore();

        applyScaleTransformationIfEnable(matrix, interpolatedTime);
        applySkewTransformationIfEnable(matrix, interpolatedTime);

        float pivotX = mInfo.basePivotX + mInfo.offsetPivotX * interpolatedTime;
        float pivotY = mInfo.basePivotY + mInfo.offsetPivotY * interpolatedTime;
        matrix.preTranslate(-pivotX, -pivotY);
        // Translate to the target origin point
        matrix.postTranslate(pivotX, pivotY);

        applyOpacityTransformation(t, interpolatedTime);

        mCurMatrix = matrix;
    }

    private void applyTranslateTransformationIfEnable(Camera camera, float interpolatedTime) {

        float x, y, z;
        x = mInfo.baseTranslateX;
        y = mInfo.baseTranslateY;
        z = mInfo.baseTranslateZ;

        if (mInfo.translateEnable) {
            x += interpolatedTime * mInfo.offsetTranslateX;
            y += interpolatedTime * mInfo.offsetTranslateY;
            z += interpolatedTime * mInfo.offsetTranslateZ;
        }
        camera.translate(x, -y, z);

    }

    private void applyRotateTransformationIfEnable(Camera camera, float interpolatedTime) {

        float x, y, z;

        x = mInfo.baseRotateX;
        y = mInfo.baseRotateY;
        z = mInfo.baseRotateZ;

        if (mInfo.rotateEnable) {
            x += interpolatedTime * mInfo.offsetRotateX;
            y += interpolatedTime * mInfo.offsetRotateY;
            z += interpolatedTime * mInfo.offsetRotateZ;
        }
        // For keeping the same rotate direction between web and android, x and z and y is negative
        camera.rotate(-x, -y, -z);
    }

    private void applyScaleTransformationIfEnable(Matrix matrix, float interpolatedTime) {

        float x, y;

        x = mInfo.baseScaleX;
        y = mInfo.baseScaleY;

        if (mInfo.scaleEnable) {
            x += interpolatedTime * mInfo.offsetScaleX;
            y += interpolatedTime * mInfo.offsetScaleY;
        }
        matrix.preScale(x, y);
    }

    private void applySkewTransformationIfEnable(Matrix matrix, float interpolatedTime) {
        float x, y;

        x = mInfo.baseSkewX;
        y = mInfo.baseSkewY;

        if (mInfo.skewEnable) {
            x += interpolatedTime * mInfo.offsetSkewX;
            y += interpolatedTime * mInfo.offsetSkewY;
        }
        matrix.preSkew((float) Math.tan(x * DEGREE_TO_ROD), (float) Math.tan(y * DEGREE_TO_ROD));
    }

    private void applyOpacityTransformation(Transformation t, float interpolatedTime) {
        float alpha = mInfo.baseOpacity;
        if (mInfo.opacityEnable) {
            alpha = mInfo.baseOpacity + interpolatedTime * mInfo.offsetOpacity;
        }
        t.setAlpha(alpha);
    }

    public Matrix getMatrix() {
        return mCurMatrix;
    }
}
