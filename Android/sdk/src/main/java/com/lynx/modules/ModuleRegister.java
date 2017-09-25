// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.modules;

import com.lynx.core.LynxRuntime;

public class ModuleRegister {

    public ModuleRegister(LynxRuntime runtime) {
        ModuleBundle bundle = new ModuleBundle(runtime.getContext());
        for (LynxModule module : bundle.getModulePacket()) {
            runtime.registerModule(module, module.moduleName());
        }
    }
}
