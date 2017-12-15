// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.ui.coordinator;

import android.text.TextUtils;

public class CrdCommandExecutor {
    private long mNativePtr;
    private String mExecutable;

    public CrdCommandExecutor(String executable) {
        if (!TextUtils.isEmpty(executable)) {
            mNativePtr = nativePrepare(executable);
            mExecutable = executable;
        }
    }

    public void update(String executable) {
        if ((mExecutable == null && !TextUtils.isEmpty(executable))
                || mExecutable.equals(executable)) {
            mExecutable = executable;
            if (mNativePtr != 0) {
                stop();
            }
            mNativePtr = nativePrepare(executable);
        }
    }

    public CrdResult executeCommand(String method, String tag, double... args) {
        if (TextUtils.isEmpty(method) || mNativePtr == 0) {
            return null;
        }
        return new CrdResult(nativeExecute(mNativePtr, method, tag, args));
    }

    public void stop() {
        nativeDestroy(mNativePtr);
        mNativePtr = 0;
        mExecutable = "";
    }

    public boolean updateProperty(String property, String value) {
        return nativeUpdateProperty(mNativePtr, property, 0, value, 0, false);
    }

    public boolean updateProperty(String property, double value) {
        return nativeUpdateProperty(mNativePtr, property, 1, null, value, false);
    }

    public boolean updateProperty(String property, boolean value) {
        return nativeUpdateProperty(mNativePtr, property, 2, null, 0, value);
    }


    private static native long nativePrepare(String executable);
    private static native void nativeDestroy(long ptr);
    private static native Object[] nativeExecute(long ptr, String method, String tag, double[] args);
    private static native boolean nativeUpdateProperty(long ptr, String property, int type, String value1, double value2, boolean value3);

}
