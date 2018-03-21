// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.ui.anim;

import com.lynx.ui.LynxUI;
import com.lynx.ui.anim.matrix.MatrixAnimDriver;

import java.util.List;

public abstract class AnimDriver {

    public static AnimDriver create(LynxUI proxy,
                                    List<AnimInfo> infoList,
                                    AnimInfo.Option option) {
        return new MatrixAnimDriver(proxy, infoList, option);
    }

    public abstract void startAnim();

    public abstract void stopAnim();

}
