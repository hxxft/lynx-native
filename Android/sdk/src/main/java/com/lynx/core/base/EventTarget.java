// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.core.base;

public interface EventTarget {
    boolean isStopPropagation();
    boolean isPreventDefault();
    boolean isCaptureEnable();
}
