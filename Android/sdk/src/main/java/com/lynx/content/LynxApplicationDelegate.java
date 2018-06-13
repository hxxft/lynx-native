// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.content;

import android.app.Application;
import android.content.Context;

import com.lynx.LynxExtModule;
import com.lynx.content.info.App;
import com.lynx.content.info.Manifest;
import com.lynx.core.LynxRuntimeManager;
import com.lynx.dev.DevSupportManager;
import com.lynx.resources.ResourceManager;

import java.util.List;

public class LynxApplicationDelegate {
    private Application mApplication;
    private Manifest mManifest;
    private List<LynxExtModule> mLynxExtModules;
    private String mManifestResourceURL = "Asset://manifest.json";
    private LynxContent mLynxContent;

    private static Context mApplicationContext = null;

    public LynxApplicationDelegate(Application application) {
        mApplication = application;
        mApplicationContext = mApplication.getApplicationContext();

        mLynxContent = new LynxContent();
    }

    public void onCreate() {
        mLynxContent.Initialize();
        LynxRuntimeManager.prepare(mApplication, mLynxExtModules);
        mManifest = ResourceManager.instance().reader().
                readResourceAsJSON(ResourceManager.toRealURL(mManifestResourceURL), Manifest.class);
        if(mManifest.isDebuggable()) {
            DevSupportManager.getInstance().initialize();
        }
    }

    public void setExtModules(List<LynxExtModule> modules){
        mLynxExtModules = modules;
    }

    public App getAppInfo() {
        return mManifest.getApplication();
    }

    public static Context getApplicationContext() {
        return mApplicationContext;
    }

    static {
        System.loadLibrary("lynx");
    }
}
