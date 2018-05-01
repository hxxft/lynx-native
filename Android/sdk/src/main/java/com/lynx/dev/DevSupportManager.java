// Copyright 2017 The Lynx Authors. All rights reserved.

package com.lynx.dev;

public class DevSupportManager extends DebugHostImpl {

    private static DevSupportManager mDevSupportManager;

    public static DevSupportManager getInstance() {
        if(mDevSupportManager == null) {
            mDevSupportManager = new DevSupportManager();
        }
        return mDevSupportManager;
    }

    public void setDebugDevHost(DebugDevHost host) {
        mHost = host;
    }

    public void initialize() {
        mDevSupportManager.nativeAttach();
    }

}
