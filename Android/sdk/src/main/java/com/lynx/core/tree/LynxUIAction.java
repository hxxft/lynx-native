// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.core.tree;

import com.lynx.core.impl.RenderObjectImpl;

public abstract class LynxUIAction {

    private LynxUIActionType mType;
    protected RenderObjectImpl mTarget;

    public LynxUIAction(LynxUIActionType type, RenderObjectImpl target) {
        mType = type;
        mTarget = target;
    }

    public LynxUIActionType type() {
        return mType;
    }

    public RenderObjectImpl target() {
        return mTarget;
    }

    public abstract void doAction();
}
