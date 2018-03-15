// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.test;

import android.app.Application;

import com.lynx.core.LynxRuntimeManager;

public class TestApplication extends Application {
    @Override
    public void onCreate() {
        super.onCreate();
        LynxRuntimeManager.prepare(this);
    }
}
