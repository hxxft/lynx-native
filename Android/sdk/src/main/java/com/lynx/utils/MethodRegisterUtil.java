// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.utils;

public class MethodRegisterUtil {
    public static native void nativeRegisterJSMethod(int type,String methodName, int methodId);
}
