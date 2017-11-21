// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.utils;

import android.app.Activity;
import android.content.Context;
import android.util.DisplayMetrics;

public class ScreenUtil {
    private static ScreenUtil sUtil;
    private float mDensity;
    private int mScreenHeight;
    private int mScreenWidth;

    private ScreenUtil(Context context) {
        DisplayMetrics metric = new DisplayMetrics();
        ((Activity)context).getWindowManager().getDefaultDisplay().getMetrics(metric);
        mDensity = metric.density;
        mScreenHeight = metric.heightPixels;
        mScreenWidth = metric.widthPixels;
    }

    public static ScreenUtil init(Context context) {
        if (sUtil == null) {
            sUtil = new ScreenUtil(context);
        }
        return sUtil;
    }

    public static int getScreenHeight() {
        return sUtil.mScreenHeight;
    }

    public static int getScreenWidth() {
        return sUtil.mScreenWidth;
    }

    public static float getScreenDensity() {
        return sUtil.mDensity;
    }
}
