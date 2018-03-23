// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.modules.ext;

import android.content.Context;

import com.lynx.core.impl.RenderObjectImpl;
import com.lynx.ui.LynxUI;

public abstract class LynxUIComponent {
    public abstract LynxUI createExtLynxUI(Context context, RenderObjectImpl impl);
    public abstract void registerMethodSpecList();
    public abstract void dispatchJSMethodWithId(LynxUI ui, int id, Object param);
}
