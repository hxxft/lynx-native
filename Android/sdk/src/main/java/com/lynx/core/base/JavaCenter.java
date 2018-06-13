// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.core.base;

import android.support.annotation.NonNull;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * Uses for saving fields and methods, which is convenient for
 * method invoking and field setting
 */

/* package */ public class JavaCenter {
    private static JavaCenter sCenter;
    // Uses for finding the matched method while params need to be transformed
    private Map<Class<?>, Map<String, List<JavaMethod>>> mMethods;
    // Uses for finding the matched method fast
    private Map<Class<?>, Map<String, JavaMethod>> mSignatureMethods;
    private Map<Class<?>, Map<String, JavaField>> mFields;

    public synchronized static JavaCenter link() {
        if (sCenter == null) {
            sCenter = new JavaCenter();
        }
        return sCenter;
    }

    public JavaCenter() {
        mMethods = new HashMap<>();
        mSignatureMethods = new HashMap<>();
        mFields = new HashMap<>();
    }

    public boolean hasRegistered(Class clazz) {
        if (mMethods.containsKey(clazz)
                || mFields.containsKey(clazz)
                || mSignatureMethods.containsKey(clazz)) {
            return true;
        }
        return false;
    }

    public Map<String, List<JavaMethod>> getMethods(Class clazz) {
        return mMethods.get(clazz);
    }

    public void registerMethod(JavaMethod method) {
        Map<String, List<JavaMethod>> methodMap = mMethods.get(method.getDeclaringClass());
        if (methodMap == null) {
            methodMap = new HashMap<>();
            mMethods.put(method.getDeclaringClass(), methodMap);
        }
        List<JavaMethod> methodList = methodMap.get(method.getName());
        if (methodList == null) {
            methodList = new ArrayList<>();
            methodMap.put(method.getName(), methodList);
        }
        methodList.add(method);

        registerAsSignatureMethod(method);
    }

    private void registerAsSignatureMethod(JavaMethod method) {
        Map<String, JavaMethod> methodMap = mSignatureMethods.get(method.getDeclaringClass());
        if (methodMap == null) {
            methodMap = new HashMap<>();
            mSignatureMethods.put(method.getDeclaringClass(), methodMap);
        }
        methodMap.put(method.getSignature(), method);
    }

    public void registerField(JavaField field) {
        Map<String, JavaField> fieldMap = mFields.get(field.getDeclaringClass());
        if (fieldMap == null) {
            fieldMap = new HashMap<>();
            mFields.put(field.getDeclaringClass(), fieldMap);
        }
        if (!fieldMap.containsKey(field.getName())) {
            fieldMap.put(field.getName(), field);
        }
    }

    public JavaMethod findMethod(Object receiver, String methodName, Object... args) {
        if (mMethods.get(receiver.getClass()) == null) {
            return null;
        }
        String signature = JavaMethod.generateSignature(methodName, args);
        // First check width signature
        JavaMethod targetMethod = mSignatureMethods.get(receiver.getClass()).get(signature);
        if (targetMethod == null) {
            // Second find match method
            targetMethod = findMatchedMethod(receiver, methodName, args);
        }
        return targetMethod;
    }

    private JavaMethod findMatchedMethod(Object receiver, String methodName, Object[] args) {

        List<JavaMethod> methods = mMethods.get(receiver.getClass()).get(methodName);
        for (JavaMethod method : methods) {
            if (method.getParameterTypes().isMatchedWith(args)) {
                return method;
            }
        }
        return null;
    }

    public JavaField findField(@NonNull Object receiver, String field) {
        return findField(receiver.getClass(), field);
    }

    public JavaField findField(@NonNull Class receiver, String field) {
        if (mFields.get(receiver) == null) {
            return null;
        }
        return mFields.get(receiver).get(field);
    }
}
