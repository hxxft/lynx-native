// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.modules.ext;

import com.lynx.core.base.LynxObject;

public class ExtLynxObject extends LynxObject {

    protected JSComponent mComponent;

    public ExtLynxObject(JSComponent component) {
        mComponent = component;
    }

    @Override
    public void onDestroy() {
        mComponent.onDestroy();
    }
}
