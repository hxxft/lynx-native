// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.content;

import android.app.Application;

import com.lynx.BuildConfig;
import com.lynx.content.info.App;
import com.lynx.content.info.Manifest;
import com.lynx.core.RuntimeManager;
import com.lynx.dev.DevSupportManager;
import com.lynx.resources.ResourceManager;

public class LynxApplicationDelegate {
    private Application mApplication;
    private Manifest mManifest;

    public LynxApplicationDelegate(Application application) {
        mApplication = application;
    }

    public void onCreate() {
        if(BuildConfig.DEBUG) {
            DevSupportManager.getInstance().initialize();
        }
        RuntimeManager.prepare(mApplication);
        mManifest = ResourceManager.instance()
                .getObjectFromGson(Manifest.MATNIFEST_URI, Manifest.class);
    }

    public App getAppInfo() {
        return mManifest.getApplication();
    }
}
