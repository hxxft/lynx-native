// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.core.base;

import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;

/**
 * 对于java的method进行的包装，方便进行方法的调用，方法唯一的识别
 */

/* package */ class JavaMethod {
    private Method mMethod;
    private String mSignature;
    private JavaProperty mParamsTypes;
    private JavaProperty mReturnType;

    public JavaMethod(Method method) {
        mMethod = method;
        mMethod.setAccessible(true);
        mParamsTypes = new JavaProperty(mMethod.getParameterTypes());
        mReturnType = new JavaProperty(mMethod.getReturnType());
        mSignature = generateSignature(this);
    }

    public Object invoke(Object receiver, Object... args) {
        try {
            return mMethod.invoke(receiver, args);
        } catch (IllegalAccessException e) {
            e.printStackTrace();
        } catch (InvocationTargetException e) {
            e.printStackTrace();
        }
        return null;
    }

    public String getName() {
        return mMethod.getName();
    }

    public JavaProperty getParameterTypes() {
        return mParamsTypes;
    }

    public int getParameterLength() {
        return mMethod.getParameterTypes().length;
    }

    public JavaProperty getReturnType() {
        return mReturnType;
    }

    public String getSignature() {
        return mSignature;
    }

    public Class getDeclaringClass() {
        return mMethod.getDeclaringClass();
    }

    public static String generateSignature(JavaMethod method) {
        if (method == null) {
            return "";
        }
        StringBuilder result = new StringBuilder();
        result.append(method.getName());
        result.append('(').append(method.getParameterTypes().getTransformTypes()).append(')');
        return result.toString();
    }

    public static String generateSignature(String methodName, Object... args) {
        StringBuilder result = new StringBuilder();
        result.append(methodName);
        result.append('(').append(JavaProperty.propertiesType(args)).append(')');
        return result.toString();
    }
}
