// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.utils;

public class PixelUtil {

    private static float sZoomReferenceDp = 750;

    public static void init(int zoomReferencePx) {
        if (zoomReferencePx < 0) {
            sZoomReferenceDp = 750;
        } else {
            sZoomReferenceDp = zoomReferencePx / ScreenUtil.getScreenDensity();
        }
    }

    public static double pxToDp(double px) {
        return px / ScreenUtil.getScreenDensity();
    }

    public static double dpToPx(double dp) {
        return dp * ScreenUtil.getScreenDensity();
    }

    public static double pxToLynxNumber(double px) {
        // Origin: px / density * (zoomReference / (screenWidth / density))
        return px * sZoomReferenceDp / ScreenUtil.getScreenWidth();
    }

    public static double dpToLynxNumber(double dp) {
        // Origin: dp * (zoomReference / (screenWidth / density))
        return dpToPx(dp) * sZoomReferenceDp / ScreenUtil.getScreenWidth();
    }

    public static double lynxNumberToPx(double number) {
        return number * ScreenUtil.getScreenWidth() / sZoomReferenceDp;
    }

    public static double lynxNumberToDp(double number) {
        return pxToDp(lynxNumberToPx(number));
    }
}
