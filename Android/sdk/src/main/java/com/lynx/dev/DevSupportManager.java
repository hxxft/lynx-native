// Copyright 2017 The Lynx Authors. All rights reserved.

package com.lynx.dev;

public class DevSupportManager {

    private DebugLogger mLogger;
    private DebugServer mDebugServer;

    private static DevSupportManager mDevSupportManager;

    public static DevSupportManager getInstance() {
        if(mDevSupportManager == null) {
            mDevSupportManager = new DevSupportManager();
        }
        return mDevSupportManager;
    }

    public void setDebugDevHost(DebugDevHost host) {
        if(mDebugServer != null) {
            mDebugServer.setDebugDevHost(host);
        }
    }

    public void initialize() {
        mDebugServer = new DebugServer();
        mDebugServer.initialize();
        mLogger = new DebugLogger();
        mLogger.start();
    }

}
