// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.ui.anim;

import com.lynx.ui.LynxUI;
import com.lynx.ui.anim.matrix.MatrixAnimDriver;
import com.lynx.ui.anim.property.PropertyAnimDriver;

public abstract class AnimDriver {

    public static AnimDriver create(LynxUI ui, String animEvent, AnimProperties from, AnimProperties to) {
        if (to.toMatrix3d != null) {
            return new MatrixAnimDriver(ui, animEvent, from, to);
        } else {
            return new PropertyAnimDriver(ui, animEvent, to);
        }
    }

    public abstract void startAnim();

    public abstract void stopAnim();

}
