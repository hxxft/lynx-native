// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.core.tree;

import com.lynx.core.impl.RenderObjectImpl;

public class LynxUIEventAction extends LynxUIAction {

    private String mEvent;
    private Object[] mParams;

    public LynxUIEventAction(RenderObjectImpl target, String event, Object[] params) {
        super(LynxUIActionType.DO_EVENT_ACTION, target);
        mEvent = event;
        mParams = params;
    }

    public String event() {
        return mEvent;
    }

    @Override
    public void doAction() {
        mTarget.dispatchEvent(mEvent, mParams);
    }
}
