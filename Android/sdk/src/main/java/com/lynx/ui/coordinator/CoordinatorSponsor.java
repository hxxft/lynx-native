// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.ui.coordinator;

import android.view.MotionEvent;

public interface CoordinatorSponsor extends CoordinatorObject {
    boolean dispatchCoordinatorScroll(int scrollTop, int scrollLeft);
    boolean dispatchCoordinatorTouch(MotionEvent event);
}
