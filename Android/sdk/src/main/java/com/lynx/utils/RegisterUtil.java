// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.utils;

public class RegisterUtil {
    public static native void nativeRegisterTag(int type, String tagName);
    public static native void nativeRegisterJSMethod(int type, String methodName, int methodId);
}
