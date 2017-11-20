// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.modules;


import com.lynx.core.LynxRuntime;

import java.util.ArrayList;
import java.util.List;

public class ModuleBundle {
    private LynxRuntime mRuntime;

    public ModuleBundle(LynxRuntime runtime) {
        mRuntime = runtime;
    }

    public List<LynxModule> getModulePacket() {
        List<LynxModule> moduleList = new ArrayList<>();
        moduleList.add(new PageNavigator(mRuntime.getContext()));
        moduleList.add(new CoordinatorRegister(mRuntime));
        return moduleList;
    }

}
