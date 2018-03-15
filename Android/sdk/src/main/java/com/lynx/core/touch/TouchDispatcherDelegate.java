// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.core.touch;

import android.support.annotation.NonNull;

import com.lynx.core.impl.RenderObjectImpl;
import com.lynx.ui.LynxUI;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public class TouchDispatcherDelegate implements TouchDispatcher.Delegate {

    @Override
    public List<TouchTarget> findPathByTarget(@NonNull TouchTarget finalTarget) {
        LynxUI ui = (LynxUI) finalTarget;
        List<TouchTarget> list = new ArrayList<>();
        RenderObjectImpl renderObject = ui.getRenderObjectImpl();
        do {
            list.add(renderObject.getUI());
            renderObject = renderObject.getParent();
        } while (renderObject != null);
        Collections.reverse(list);
        return list;
    }

}
