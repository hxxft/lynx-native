// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.ui.anim.matrix;

import com.lynx.base.Style;
import com.lynx.core.base.LynxObject;

/**
 *
 * The function of this class is different from Matrix Parser which
 * is hard to control and costs redundant calculation. So this just
 * handle action from js like translateX / scrollX / skewX, and the
 * relation between previous action and current action.
 *
 * Maybe at the next version, we will actually handle the Matrix Parser.
 *
 */

public class Matrix3dParser {

    private static final float CAMERA_DISTANCE_FACTOR = 1F / 10;

    private static final String TRANSLATE_X = "translateX";
    private static final String TRANSLATE_Y = "translateY";
    private static final String TRANSLATE_Z = "translateZ";
    private static final String SCALE_X = "scaleX";
    private static final String SCALE_Y = "scaleY";
    private static final String SCALE_Z = "scaleZ";
    private static final String ROTATE_X = "rotateX";
    private static final String ROTATE_Y = "rotateY";
    private static final String ROTATE_Z = "rotateZ";
    private static final String SKEW_X = "skewX";
    private static final String SKEW_Y = "skewY";

    public static AnimInformation parser(LynxObject from, LynxObject to,
                                         float fromOpacity, float toOpacity, float perspective) {

        if (to == null) {
            return null;
        }

        AnimInformation info = new AnimInformation();

        fillBaseInfo(from, info);

        checkIfTranslate(to, info);
        checkIfRotate(to, info);
        checkIfScale(to, info);
        checkIfSkew(to, info);
        checkIfOpacity(fromOpacity, toOpacity, info);

        // Scale the perspective so that the animation can be the same as web
        info.perspective = (float) (perspective * CAMERA_DISTANCE_FACTOR / Style.sDensity);

        return info;
    }

    private static void fillBaseInfo(LynxObject from, AnimInformation info) {
        if (from == null) {
            return;
        }

        // TODO 需要将底层传过来的sp转化成px
        info.baseTranslateX = XCast.toFloat(from.getProperty(TRANSLATE_X), 0);
        info.baseTranslateY = XCast.toFloat(from.getProperty(TRANSLATE_Y), 0);
        info.baseTranslateZ = XCast.toFloat(from.getProperty(TRANSLATE_Z), 0);

        info.baseScaleX = XCast.toFloat(from.getProperty(SCALE_X), 1);
        info.baseScaleY = XCast.toFloat(from.getProperty(SCALE_Y), 1);
        info.baseScaleZ = XCast.toFloat(from.getProperty(SCALE_Z), 1);

        info.baseRotateX = XCast.toFloat(from.getProperty(ROTATE_X), 0);
        info.baseRotateY = XCast.toFloat(from.getProperty(ROTATE_Y), 0);
        info.baseRotateZ = XCast.toFloat(from.getProperty(ROTATE_Z), 0);

        info.baseSkewX = XCast.toFloat(from.getProperty(SKEW_X), 0);
        info.baseSkewY = XCast.toFloat(from.getProperty(SKEW_Y), 0);
    }

    private static void checkIfRotate(LynxObject object, AnimInformation info) {

        float angleX = XCast.toFloat(object.getProperty(ROTATE_X), 0);
        float angleY = XCast.toFloat(object.getProperty(ROTATE_Y), 0);
        float angleZ = XCast.toFloat(object.getProperty(ROTATE_Z), 0);

        if (angleX != info.baseRotateX ||
                angleY != info.baseRotateY ||
                angleZ != info.baseRotateZ) {
            info.rotateEnable = true;

            // A special treatment:
            // The rotate angle from js is 0 to 0 (360), so 0 maybe means 360 degree.
            // When the value is 0, comparing with the base value, and decide if it is
            // a '0' or a '360' with the rule choosing the min D-value.
            angleX = minDValueRule(angleX, info.baseRotateX);
            angleY = minDValueRule(angleY, info.baseRotateY);
            angleZ = minDValueRule(angleZ, info.baseRotateZ);

            info.rotateX = angleX - info.baseRotateX;
            info.rotateY = angleY - info.baseRotateY;
            info.rotateZ = angleZ - info.baseRotateZ;
        }
    }

    private static float minDValueRule(float a, float base) {
        if (a == 0) {
            if (base > 180) {
                a = 360;
            } else {
                a = 0;
            }
        }
        return a;
    }

    private static void checkIfScale(LynxObject object, AnimInformation info) {

        float scaleX = XCast.toFloat(object.getProperty(SCALE_X), 1);
        float scaleY = XCast.toFloat(object.getProperty(SCALE_Y), 1);
        float scaleZ = XCast.toFloat(object.getProperty(SCALE_Z), 1);

        if (scaleX != info.baseScaleX ||
                scaleY != info.baseScaleY ||
                scaleZ != info.baseScaleZ) {
            info.scaleEnable = true;
            info.scaleX = scaleX - info.baseScaleX;
            info.scaleY = scaleY - info.baseScaleY;
            info.scaleZ = scaleZ - info.baseScaleZ;
        }
    }

    private static void checkIfTranslate(LynxObject object, AnimInformation info) {
        float translateX = XCast.toFloat(object.getProperty(TRANSLATE_X), 0);
        float translateY = XCast.toFloat(object.getProperty(TRANSLATE_Y), 0);
        float translateZ = XCast.toFloat(object.getProperty(TRANSLATE_Z), 0);

        if (translateX != info.baseTranslateX ||
                translateY != info.baseTranslateY ||
                translateZ != info.baseTranslateZ) {
            info.translateEnable = true;
            // TODO 需要将底层传过来的sp转化成px
            info.translateX = translateX - info.baseTranslateX;
            info.translateY = translateY - info.baseTranslateY;
            info.translateZ = translateZ - info.baseTranslateZ;
        }
    }

    private static void checkIfSkew(LynxObject object, AnimInformation info) {
        float skewX = XCast.toFloat(object.getProperty(SKEW_X), 0);
        float skewY = XCast.toFloat(object.getProperty(SKEW_Y), 0);

        if (skewX != info.baseSkewX ||
                skewY != info.baseSkewY) {
            info.skewEnable = true;
            info.skewX = skewX - info.baseSkewX;
            info.skewY = skewY - info.baseSkewY;
        }
    }

    private static void checkIfOpacity(float fromOpacity, float toOpacity, AnimInformation info) {

        info.baseOpacity = fromOpacity;
        info.opacity = toOpacity - fromOpacity;

        if (info.opacity > 0) {
            info.opacityEnable = true;
        }
    }

}
