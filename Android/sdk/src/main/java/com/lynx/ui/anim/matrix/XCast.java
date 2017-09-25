// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.ui.anim.matrix;

public class XCast {

    public static float toFloat(Object value, float defaultValue) {
        if (value instanceof Integer) {
            return (int) value;
        } else if (value instanceof Double) {
            return (float) (double) value;
        } else if (value instanceof Float) {
            return (float) value;
        }
        return defaultValue;
    }

    public static int toInt(Object value, int defaultValue) {
        if (value instanceof Integer) {
            return (int) value;
        } else if (value instanceof Double) {
            return (int) (double) value;
        } else if (value instanceof Float) {
            return (int) value;
        }
        return defaultValue;
    }

    public static boolean toBoolean(Object value, boolean defaultValue) {
        if (value instanceof Boolean) {
            return (Boolean) value;
        }
        return defaultValue;
    }

    public static String toString(Object value, String defaultValue) {
        if (value instanceof String) {
            return (String) value;
        }
        return defaultValue;
    }
}
