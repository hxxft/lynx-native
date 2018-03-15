// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.core.touch;

import com.lynx.core.touch.gesture.GestureWatcher;

public interface TouchTarget extends GestureWatcher {
    void performTouch(TouchEventInfo info);
    void onCapturingTouchEvent(TouchEventInfo info);
    TouchTarget hitTest(TouchAxis axis);
}
