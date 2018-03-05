// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.core;

import android.content.Context;

import com.lynx.base.Style;
import com.lynx.net.NetRequestManager;
import com.lynx.net.cookie.LynxCookieStore;
import com.lynx.resources.ResourceManager;
import com.lynx.utils.ScreenUtil;

public class LynxRuntimeManager {

    private static boolean mInited = false;

    private static LynxRuntime mIdleRuntime = null;

    public static void prepare(Context context) {
        prepare(context, LynxRuntime.DEFAULT_ZOOM_REF);
    }

    public static void prepare(Context context, int zoomReference) {
        ScreenUtil.init(context);
        Style.init(ScreenUtil.getScreenDensity());
        NetRequestManager.init(context);
        LynxCookieStore.initInstance(context);
        ResourceManager.init(context);
        LynxRuntime.prepare(zoomReference);
        initialize();
    }

    private static synchronized void initialize() {
        if(LynxRuntime.IsMemoryCheckEnabled()) return;
        mIdleRuntime = new LynxRuntime();
        mIdleRuntime.initialize();
        mInited = true;
    }

    public static synchronized LynxRuntime getIdleRuntime() {
        if(LynxRuntime.IsMemoryCheckEnabled()) {
            mIdleRuntime = new LynxRuntime();
            mIdleRuntime.initialize();
            return mIdleRuntime;
        }else {
            if (!mInited) initialize();
            LynxRuntime runtime = mIdleRuntime;
            mIdleRuntime = new LynxRuntime();
            mIdleRuntime.initialize();
            return runtime;
        }
    }

    static {
        System.loadLibrary("lynx");
    }
}
