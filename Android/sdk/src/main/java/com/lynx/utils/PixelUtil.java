// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.utils;

public class PixelUtil {

    private final static int LYNX_DEFAULT_REFER_SCREEN_WIDTH = 750;

    public static double pxToDp(double px) {
        return px / ScreenUtil.getScreenDensity();
    }

    public static double dpToPx(double dp) {
        return dp * ScreenUtil.getScreenDensity();
    }

    public static double pxToLynxNumber(double px) {
        // Origin: px / density * (LYNX_DEFAULT_REFER_SCREEN_WIDTH / (screenWidth / density))
        return px * LYNX_DEFAULT_REFER_SCREEN_WIDTH / ScreenUtil.getScreenWidth();
    }

    public static double dpToLynxNumber(double dp) {
        // Origin: dp * (LYNX_DEFAULT_REFER_SCREEN_WIDTH / (screenWidth / density))
        return dpToPx(dp) * LYNX_DEFAULT_REFER_SCREEN_WIDTH / ScreenUtil.getScreenWidth();
    }
}
