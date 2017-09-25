// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.core;

import android.content.Context;
import android.view.View;

import com.lynx.base.Style;
import com.lynx.core.base.LynxFunctionObject;
import com.lynx.core.tree.LynxRenderTreeHostImpl;
import com.lynx.modules.ModuleRegister;

public class LynxRuntime {

    private static native boolean nativeCheckMemoryEnabled();

    private static native void nativeCheckMemoryLeak();

    private native int nativeCreateNativeJSRuntime();

    private native void nativeDestroyNativeJSRuntime(long runtime);

    private native void nativeRunScript(long runtime, String source);

    private native void nativeLoadHTML(long runtime, String url, String source);

    private native void nativeLoadUrl(long runtime, String url);

    private native void nativeInitRuntime(long runtime);

    private native Object nativeActiveRuntime(long runtime, int width, int height, double density);

    private native String nativeGetPageURL(long runtime);

    private native String nativeGetUserAgent(long runtime);

    private native void nativeAddJavascriptInterface(long runtime, LynxFunctionObject object, String name);

    private long mNativeRuntime;

    private static boolean mEnableMemoryCheck = false;

    private Context mContext;

    public LynxRuntime() {
        mNativeRuntime = nativeCreateNativeJSRuntime();
    }

    public void runScript(String source) {
        nativeRunScript(mNativeRuntime, source);
    }

    public void loadHTML(String url, String source) {
        nativeLoadHTML(mNativeRuntime, url, source);
    }

    public void loadUrl(String url) {
        nativeLoadUrl(mNativeRuntime, url);
    }

    public void initialize() {
        nativeInitRuntime(mNativeRuntime);
    }

    public LynxRenderTreeHostImpl active(View view,
                                         int width,
                                         int height,
                                         double density) {
        mContext = view.getContext();
        ModuleRegister register = new ModuleRegister(this);
        Object host = nativeActiveRuntime(mNativeRuntime, width, height, density);
        Style.sDensity = density;
        return (LynxRenderTreeHostImpl) host;
    }

    public void registerModule(LynxFunctionObject object, String name) {
        nativeAddJavascriptInterface(mNativeRuntime,
                new LynxFunctionObject(object), name);
    }

    public void destroy() {

        nativeDestroyNativeJSRuntime(mNativeRuntime);
        mNativeRuntime = 0;

        if(mEnableMemoryCheck) {
            checkMemoryLeak();
        }
    }

    public String getUserAgent() {
        return nativeGetUserAgent(mNativeRuntime);
    }

    public String getPageURL() {
        return nativeGetPageURL(mNativeRuntime);
    }

    public long id() {
        return mNativeRuntime;
    }

    public Context getContext() {
        return mContext;
    }

    void checkMemoryLeak() {
        new Thread(new Runnable() {
            @Override
            public void run() {
                try {
                    Thread.sleep(3000);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
                nativeCheckMemoryLeak();
            }
        }).start();
    }

    static boolean IsMemoryCheckEnabled() {
        return mEnableMemoryCheck;
    }

    static void checkMemoryEnabled() {
        mEnableMemoryCheck = nativeCheckMemoryEnabled();
    }
}
