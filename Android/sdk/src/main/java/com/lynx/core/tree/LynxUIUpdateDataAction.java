// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.core.tree;

import com.lynx.core.impl.RenderObjectAttr;
import com.lynx.core.impl.RenderObjectImpl;

public class LynxUIUpdateDataAction extends LynxUIAction {
    private RenderObjectAttr mKey;
    private Object mParam;

    public LynxUIUpdateDataAction(RenderObjectImpl target,
                                  RenderObjectAttr key,
                                  Object param) {
        super(LynxUIActionType.DO_UPDATE_DATA_ACTION, target);
        mKey = key;
        mParam = param;
    }

    public RenderObjectAttr key() {
        return mKey;
    }

    @Override
    public void doAction() {
        mTarget.updateData(mKey, mParam);
    }
}
