// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.ui.anim;

import com.lynx.core.base.LynxArray;
import com.lynx.core.base.LynxObject;
import com.lynx.ui.anim.matrix.XCast;

public class AnimProperties {

    private static final int DEFAULT_PERSPECTIVE = 0;
    private static final int DISABLE_PERSPECTIVE = Integer.MAX_VALUE;
    private static final int DEFAULT_OPACITY = 1;
    private static final float DEFAULT_TRANSFORM_ORIGIN = 0.5f;

    public long time;
    public String type;
    public float fromLeft;
    public float toLeft;
    public float fromTop;
    public float toTop;
    public float fromXScale;
    public float toXScale;
    public float fromYScale;
    public float toYScale;
    public float fromOpacity;
    public float toOpacity;
    // Matrix animation params
    public LynxObject toMatrix3d;
    // Perspective distance on Z direction
    public int perspective;
    // Transform origin axis
    public LynxArray transformOrigin;
    // Enable the perspective mode
    public boolean enablePerspective;

    public static AnimProperties createFrom(LynxObject object) {

        if (object == null) {
            return null;
        }

        AnimProperties properties = new AnimProperties();

        properties.type = XCast.toString(object.getProperty("type"), "");
        properties.time = XCast.toInt(object.getProperty("time"), 0);

        properties.fromLeft = XCast.toFloat(object.getProperty("from_left"), 0);
        properties.toLeft = XCast.toFloat(object.getProperty("to_left"), 0);

        properties.fromTop = XCast.toFloat(object.getProperty("from_top"), 0);
        properties.toTop = XCast.toFloat(object.getProperty("to_top"), 0);

        properties.fromXScale = XCast.toFloat(object.getProperty("from_xscale"), 0);
        properties.toXScale = XCast.toFloat(object.getProperty("to_xscale"), 0);

        properties.fromYScale = XCast.toFloat(object.getProperty("from_yscale"), 0);
        properties.toYScale = XCast.toFloat(object.getProperty("to_yscale"), 0);

        properties.fromOpacity = XCast.toFloat(object.getProperty("from_opacity"), DEFAULT_OPACITY);
        properties.toOpacity = XCast.toFloat(object.getProperty("to_opacity"), DEFAULT_OPACITY);

        properties.enablePerspective = XCast.toBoolean(object.getProperty("enablePerspective"), false);

        if (!properties.enablePerspective) {
            // Disable perspective mode with the distance far away enough,
            properties.perspective = DISABLE_PERSPECTIVE;
        } else {
            properties.perspective = XCast.toInt(object.getProperty("perspective"), DEFAULT_PERSPECTIVE);
        }

        Object temp;

        temp = object.getProperty("to_matrix3d");
        if (temp != null) {
            properties.toMatrix3d = (LynxObject) temp;
        }

        temp = object.getProperty("transformOrigin");
        if (temp != null) {
            properties.transformOrigin = (LynxArray) temp;
        } else {
            properties.transformOrigin = new LynxArray();
            properties.transformOrigin.add(DEFAULT_TRANSFORM_ORIGIN);
            properties.transformOrigin.add(DEFAULT_TRANSFORM_ORIGIN);
        }

        return properties;
    }

    public AnimProperties() {
    }
}
