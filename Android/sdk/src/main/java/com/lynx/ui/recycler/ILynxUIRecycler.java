// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.ui.recycler;

import com.lynx.ui.LynxUI;

/**
 * Implementing ths interface means the {@link LynxUI} has function of recycling children.
 */

public interface ILynxUIRecycler {

    void notifyChildrenChanged();
}
