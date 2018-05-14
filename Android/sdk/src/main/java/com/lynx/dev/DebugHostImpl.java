package com.lynx.dev;

import android.app.Activity;

import com.lynx.base.CalledByNative;

import java.util.HashMap;
import java.util.Map;

/**
 * Created by dli on 2018/4/30.
 */

public class DebugHostImpl {

    public final static int DEBUG_TYPE_RELOAD = 0;

    protected DebugDevHost mHost = null;


    @CalledByNative
    public void runDebug(int type) {
        if(mHost == null) return;
        switch (type) {
            case DEBUG_TYPE_RELOAD:
                mHost.debugHotReload("Debug://");
                break;
            default:
                break;
        }
    }

    protected native void nativeAttach();
}
