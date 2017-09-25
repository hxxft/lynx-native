// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.ui.recycler;

public interface HiSpeedStopLoadItem {

    void stopLoadResWhenViewInHiSpeed();

    void allowLoadResWhenViewInHiSpeed();

    void reloadResWhenSuitable();

}
