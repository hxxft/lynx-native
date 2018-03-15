// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.core.base;

import android.support.annotation.Keep;

public class LynxHolder {
    private long mNativeHolder;

    public LynxHolder(long nativeHolder) {
        mNativeHolder = nativeHolder;
    }

    @Keep
    public long getNativeHolder() {
        return mNativeHolder;
    }
}
