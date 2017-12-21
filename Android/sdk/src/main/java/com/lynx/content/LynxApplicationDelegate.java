// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.content;

import android.app.Application;

import com.lynx.content.info.App;
import com.lynx.content.info.Manifest;
import com.lynx.core.LynxRuntimeManager;
import com.lynx.dev.DevSupportManager;
import com.lynx.resources.ResourceManager;

public class LynxApplicationDelegate {
    private Application mApplication;
    private Manifest mManifest;
    private String mManifestResourceURL = "Asset://manifest.json";

    public LynxApplicationDelegate(Application application) {
        mApplication = application;
    }

    public void onCreate() {

        LynxRuntimeManager.prepare(mApplication);
        mManifest = ResourceManager.instance().reader().
                readResourceAsJSON(ResourceManager.toRealURL(mManifestResourceURL), Manifest.class);
        if(mManifest.isDebuggable()) {
            DevSupportManager.getInstance().initialize();
        }
    }

    public App getAppInfo() {
        return mManifest.getApplication();
    }
}
