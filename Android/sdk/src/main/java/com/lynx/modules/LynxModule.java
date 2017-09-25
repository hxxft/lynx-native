// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.modules;

import com.lynx.core.base.LynxFunctionObject;

public abstract class LynxModule extends LynxFunctionObject {
    private boolean mHasBeenUsed = false;

    public LynxModule() {
    }

    public void initialize() {

    }

    public abstract String moduleName();

    @Override
    public Object exec(String methodName, Object[] args) {
        if (!mHasBeenUsed) {
            initialize();
            mHasBeenUsed = true;
        }
        return super.exec(methodName, args);
    }
}
