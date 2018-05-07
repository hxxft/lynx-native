// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.core.base;

import com.lynx.base.CalledByNative;

public class ParamsTransform {

    @CalledByNative
    public static String transform(Object[] params) {
        if (params != null) {
            StringBuilder builder = new StringBuilder();
            for (int i = 0; i < params.length; i++) {
                builder.append(transform(params[i]));
            }
            return builder.toString();
        }
        return "";
    }

    @CalledByNative
    public static String transform(Object param) {
        if (param == null) {
            return JNIType.VOID_TYPE;
        }
        Class clazz;
        if (param instanceof Class) {
            clazz = (Class) param;
        } else {
            clazz = param.getClass();
        }
        if (clazz == void.class) {
            return JNIType.VOID_TYPE;
        } if (clazz.getComponentType() != null) {
            String type = JNIType.ARRAY_FLAG + transform(clazz.getComponentType());
            if (type.length() <= 2) return type;
        }else if (clazz == short.class || clazz == Short.class) {
            return JNIType.SHORT_TYPE;
        } else if (clazz == int.class || clazz == Integer.class) {
            return JNIType.INI_TYPE;
        } else if (clazz == long.class || clazz == Long.class) {
            return JNIType.LONG_TYPE;
        } else if (clazz == float.class || clazz == Float.class) {
            return JNIType.FLOAT_TYPE;
        } else if (clazz == double.class || clazz == Double.class) {
            return JNIType.DOUBLE_TYPE;
        } else if (clazz == char.class || clazz == Character.class) {
            return JNIType.CHAR_TYPE;
        } else if (clazz == boolean.class || clazz == Boolean.class) {
            return JNIType.BOOLEAN_TYPE;
        } else if (clazz == byte.class || clazz == Byte.class) {
            return JNIType.BYTE_TYPE;
        } else if (clazz == String.class) {
            return JNIType.STRING_TYPE;
        } else if (LynxArray.class.isAssignableFrom(clazz)) {
            return JNIType.LYNX_ARRAY_TYPE;
        } else if (LynxMap.class.isAssignableFrom(clazz)) {
            return JNIType.LYNX_OBJECT_TYPE;
        } else if (clazz == LynxObject.class) {
            return JNIType.LYNX_FUNCTION_OBJECT_TYPE;
        } else if (LynxHolder.class.isAssignableFrom(clazz)) {
            return JNIType.LYNX_HOLDER;
        }
        throw new RuntimeException(clazz.getCanonicalName() + " is not defined as a property to lynx jni !");
    }

}
