// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.modules.ext;

import com.lynx.core.LynxRuntime;

public abstract class JSComponent {
    private boolean mHasBeenUsed = false;

    public JSComponent(LynxRuntime runtime) {
    }

    public void initialize() {

    }


    public Object exec(String methodName, Object[] args) {
        if (!mHasBeenUsed) {
            initialize();
            mHasBeenUsed = true;
        }
        return null;
    }


    public void onDestroy() {
    }


}
