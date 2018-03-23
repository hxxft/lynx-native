// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.modules.ext;

import com.lynx.core.base.LynxFunctionObject;

public class ExtLynxFunctionObject extends LynxFunctionObject {

    protected JSComponent mComponent;

    public ExtLynxFunctionObject(JSComponent component) {
        mComponent = component;
    }

    @Override
    public void onDestroy() {
        mComponent.onDestroy();
    }
}
