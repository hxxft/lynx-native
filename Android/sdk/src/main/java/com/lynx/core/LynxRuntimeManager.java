// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.core;

import android.content.Context;

import com.lynx.base.Style;
import com.lynx.modules.ext.LynxModule;
import com.lynx.modules.ext.ModuleManager;
import com.lynx.net.NetRequestManager;
import com.lynx.net.cookie.LynxCookieStore;
import com.lynx.resources.ResourceManager;
import com.lynx.utils.PixelUtil;
import com.lynx.utils.ScreenUtil;

import java.util.List;

public class LynxRuntimeManager {

    private static boolean mInited = false;

    private static LynxRuntime mIdleRuntime = null;

    public static void prepare(Context context, List<? extends LynxModule> modules) {
        prepare(context, LynxRuntime.DEFAULT_ZOOM_REF, modules);
    }

    public static void prepare(Context context, int zoomRatioPx) {
        prepare(context, zoomRatioPx, null);
    }
    public static void prepare(Context context, int zoomRatioPx,
                               List<? extends LynxModule> modules) {
        ScreenUtil.init(context);
        PixelUtil.init(zoomRatioPx);
        Style.init(ScreenUtil.getScreenDensity());
        NetRequestManager.init(context);
        LynxCookieStore.initInstance(context);
        ResourceManager.init(context);
        LynxRuntime.prepare(zoomRatioPx);
        initialize();
        ModuleManager.registerExtModule(modules);
        ModuleManager.createUIComponent();
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
}
