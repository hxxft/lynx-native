// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.modules.ext;

import com.lynx.core.LynxRuntime;

import java.util.HashMap;
import java.util.Map;

public class LynxModule {
    public Map<Integer, LynxUIComponent> createUIComponents() {
        return new HashMap<>();
    }

    public void registerFunctionObject(LynxRuntime runtime) {
    }
}
