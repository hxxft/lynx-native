// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.ui.coordinator;

import android.text.TextUtils;

public class CommandExecutor {
    private long mNativePtr;
    private String mExecutable;

    public CommandExecutor(String executable) {
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

    public CoordinatorResult executeCommand(String method, String tag, double... args) {
        if (TextUtils.isEmpty(method) || mNativePtr == 0) {
            return null;
        }
        return new CoordinatorResult(nativeExecute(mNativePtr, method, tag, args));
    }

    public void stop() {
        nativeDestroy(mNativePtr);
        mNativePtr = 0;
        mExecutable = "";
    }

    private static native long nativePrepare(String executable);
    private static native void nativeDestroy(long ptr);
    private static native Object[] nativeExecute(long ptr, String method, String tag, double[] args);

}
