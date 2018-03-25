// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.utils;

public class PixelUtil {

    private static float sZoomRatioDp = 750;

    public static void init(int zoomRatioPx) {
        if (zoomRatioPx < 0) {
            sZoomRatioDp = 750;
        } else {
            sZoomRatioDp = zoomRatioPx / ScreenUtil.getScreenDensity();
        }
    }

    public static double pxToDp(double px) {
        return px / ScreenUtil.getScreenDensity();
    }

    public static double dpToPx(double dp) {
        return dp * ScreenUtil.getScreenDensity();
    }

    public static double pxToLynxNumber(double px) {
        // Origin: px / density * (zoomRatio / (screenWidth / density))
        return px * sZoomRatioDp / ScreenUtil.getScreenWidth();
    }

    public static double dpToLynxNumber(double dp) {
        // Origin: dp * (zoomRatio / (screenWidth / density))
        return dpToPx(dp) * sZoomRatioDp / ScreenUtil.getScreenWidth();
    }

    public static double lynxNumberToPx(double number) {
        return number * ScreenUtil.getScreenWidth() / sZoomRatioDp;
    }

    public static double lynxNumberToDp(double number) {
        return pxToDp(lynxNumberToPx(number));
    }
}
