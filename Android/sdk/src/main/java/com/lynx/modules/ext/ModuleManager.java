// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.modules.ext;

import android.content.Context;
import android.support.annotation.Nullable;
import com.lynx.core.LynxRuntime;
import com.lynx.core.impl.RenderObjectImpl;
import com.lynx.ui.LynxUI;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class ModuleManager {
    private static List<LynxModule> gModules =new ArrayList<>();
    private static Map<Integer, LynxUIComponent> gUIComponents = new HashMap<>();

    public static void registerExtModule(List<? extends LynxModule> extModules) {
        if (extModules != null && extModules.size() > 0) {
            gModules.addAll(extModules);
        }
    }

    @Nullable
    public static LynxUI createExtLynxUI(int type, Context context, RenderObjectImpl impl) {
        LynxUIComponent component = gUIComponents.get(type);
        LynxUI lynxUI = null;
        if (component != null) {
            lynxUI = component.createExtLynxUI(context, impl);
        }
        return lynxUI;
    }

    public static   void callById(int type, LynxUI ui, int id, Object param) {
        LynxUIComponent component = gUIComponents.get(type);
        if (component != null) {
            component.dispatchJSMethodWithId(ui, id, param);
        }
    }

    public static void createUIComponent() {
        for (LynxModule m : gModules) {
            gUIComponents.putAll(m.createUIComponents());
        }
    }

    public static void registerJSObject(LynxRuntime runtime) {
        for (LynxModule m : gModules) {
            m.registerFunctionObject(runtime);
        }
    }
}
