package com.lynx.core.touch.gesture;

import com.lynx.core.touch.EventTarget;

public interface GestureWatcher extends EventTarget {
    void performGesture(GestureEventInfo info);
    void onCaptureGestureEvent(GestureEventInfo info);
}
