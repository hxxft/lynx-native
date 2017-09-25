// Copyright 2017 The Lynx Authors. All rights reserved.

package com.lynx.example;

import android.app.Application;

import com.lynx.content.LynxApplication;
import com.lynx.content.LynxApplicationDelegate;
import com.lynx.content.info.App;

/**
 * Created by yanxing on 17/9/20.
 */

public class MainApplication extends Application implements LynxApplication {
    private LynxApplicationDelegate mDelegate;

    @Override
    public void onCreate() {
        super.onCreate();
        mDelegate = new LynxApplicationDelegate(this);
        mDelegate.onCreate();
    }

    @Override
    public App getAppInfo() {
        return mDelegate.getAppInfo();
    }
}
