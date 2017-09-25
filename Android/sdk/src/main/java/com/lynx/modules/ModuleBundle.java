// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.modules;

import android.content.Context;

import java.util.ArrayList;
import java.util.List;

public class ModuleBundle {
    private Context mContext;

    public ModuleBundle(Context context) {
        mContext = context;
    }

    public List<LynxModule> getModulePacket() {
        List<LynxModule> moduleList = new ArrayList<>();
        moduleList.add(new PageNavigator(mContext));
        return moduleList;
    }

}
