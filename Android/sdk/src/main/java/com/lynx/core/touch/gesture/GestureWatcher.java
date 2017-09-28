package com.lynx.core.touch.gesture;

import com.lynx.core.base.EventTarget;

public interface GestureWatcher extends EventTarget {
    void performGesture(GestureEventInfo info);
}
