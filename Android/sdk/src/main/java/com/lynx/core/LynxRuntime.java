// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.core;

import android.content.Context;
import android.view.View;

import com.lynx.core.base.LynxFunctionObject;
import com.lynx.core.tree.LynxRenderTreeHostImpl;
import com.lynx.modules.ModuleRegister;
import com.lynx.utils.DeviceInfoUtil;
import com.lynx.utils.ScreenUtil;

public class LynxRuntime {

    public final static int DEFAULT_ZOOM_REF = -1;

    private static native boolean nativeCheckMemoryEnabled();

    private static native void nativeCheckMemoryLeak();

    private static native void nativeInitGlobalConfig(int screenWidth,
                                                      int screenHeight,
                                                      double density,
                                                      int zoomReference,
                                                      String deviceInfo);

    private native int nativeCreateNativeJSRuntime();

    private native void nativeDestroyNativeJSRuntime(long runtime);

    private native void nativeRunScript(long runtime, String source, Object callback);

    private native void nativeLoadHTML(long runtime, String url, String source);

    private native void nativeLoadScriptDataWithBaseUrl(long runtime, String source, String url);

    private native void nativeLoadUrl(long runtime, String url);

    private native Object nativeInitRuntime(long runtime);

    private native String nativeGetPageURL(long runtime);

    private native String nativeGetUserAgent(long runtime);

    private native void nativeSetUserAgent(long runtime, String ua);

    private native void nativeAddJavascriptInterface(long runtime, LynxFunctionObject object,
                                                     String name);

    private native void nativeSetExceptionListner(long runtime, Object listener);

    private long mNativeRuntime;

    private static boolean mEnableMemoryCheck = false;

    private Context mContext;

    private LynxRenderTreeHostImpl mHost;

    public LynxRuntime() {
        mNativeRuntime = nativeCreateNativeJSRuntime();
    }

    public static void prepare(int zoomReferencePx) {
        checkMemoryEnabled();
        nativeInitGlobalConfig(ScreenUtil.getScreenWidth(),
                ScreenUtil.getScreenHeight(),
                ScreenUtil.getScreenDensity(),
                zoomReferencePx,
                DeviceInfoUtil.getInfo());
    }

    public void runScript(String source) {
        nativeRunScript(mNativeRuntime, source, null);
    }

    public void runScript(String source, ResultCallback callback) {
        nativeRunScript(mNativeRuntime, source, callback);
    }

    public void loadScriptDataWithBaseUrl(String data, String baseUrl) {
        nativeLoadScriptDataWithBaseUrl(mNativeRuntime, data, baseUrl);
    }

    public void loadHTML(String url, String source) {
        nativeLoadHTML(mNativeRuntime, url, source);
    }

    public void loadUrl(String url) {
        nativeLoadUrl(mNativeRuntime, url);
    }

    public void initialize() {
        mHost = (LynxRenderTreeHostImpl) nativeInitRuntime(mNativeRuntime);
    }

    public LynxRenderTreeHostImpl active(View view) {
        mContext = view.getContext();
        ModuleRegister register = new ModuleRegister(this);
        return mHost;
    }

    public void registerModule(LynxFunctionObject object, String name) {
        nativeAddJavascriptInterface(mNativeRuntime, object, name);
    }

    public void addJavascriptInterface(Object object, String name) {
        nativeAddJavascriptInterface(mNativeRuntime, new LynxFunctionObject(object), name);
    }

    public void destroy() {

        nativeDestroyNativeJSRuntime(mNativeRuntime);
        mNativeRuntime = 0;

        if(mEnableMemoryCheck) {
            checkMemoryLeak();
        }
    }

    public void setUserAgent(String userAgent) {
        nativeSetUserAgent(mNativeRuntime, userAgent);
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

    public LynxRenderTreeHostImpl getHost() {
        return mHost;
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

    public void setExceptionListener(final ExceptionListener listener) {
        nativeSetExceptionListner(mNativeRuntime, new ResultCallback() {
            @Override
            public void onReceiveResult(Object result) {
                listener.onException((String) result);
            }
        });
    }

    static boolean IsMemoryCheckEnabled() {
        return mEnableMemoryCheck;
    }

    static void checkMemoryEnabled() {
        mEnableMemoryCheck = nativeCheckMemoryEnabled();
    }

}
