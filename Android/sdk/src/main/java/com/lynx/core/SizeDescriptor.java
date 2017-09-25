// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.core;

public class SizeDescriptor {
    static final int MODE_SHIFT = 30;
    static final int MODE_MASK  = 0x3 << MODE_SHIFT;
    static final int EXACTLY     = 1 << MODE_SHIFT;
    static final int AT_MOST     = 2 << MODE_SHIFT;
}
