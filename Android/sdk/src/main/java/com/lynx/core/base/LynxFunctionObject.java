// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.core.base;

import android.util.Log;
import android.webkit.JavascriptInterface;

import com.lynx.base.CalledByNative;
import com.lynx.core.LynxMethod;

import java.lang.reflect.Method;
import java.util.List;
import java.util.Map;
import java.util.Set;

public class LynxFunctionObject {
    private LynxArray mMethodNameArray;
    private Object mReceiver;

    public LynxFunctionObject() {
        initWithReceiver(this);
    }

    public LynxFunctionObject(Object receiver) {
        initWithReceiver(receiver);
    }

    private void initWithReceiver(Object receiver) {
        mReceiver = receiver;
        mMethodNameArray = new LynxArray();
        if (!JavaCenter.link().hasRegistered(receiver.getClass())) {
            Method[] methods = receiver.getClass().getDeclaredMethods();
            for (Method method : methods) {
                if (extractJSMethod(method)
                        || extractJavaScriptInterfaceMethod(method)) {
                }
            }
        } else {
            Map<String, List<JavaMethod>> methodNames = JavaCenter.link()
                    .getMethods(receiver.getClass());
            Set<Map.Entry<String, List<JavaMethod>>> entrySet = methodNames.entrySet();
            for (Map.Entry<String, List<JavaMethod>> entry : entrySet) {
                List<JavaMethod> list = entry.getValue();
                if (list.size() > 0) {
                    registerJSMethod(list.get(0));
                }
            }
        }
    }

    private boolean extractJSMethod(Method method) {
        LynxMethod annotation = method.getAnnotation(LynxMethod.class);
        if (annotation != null) {
            JavaMethod methodAfterAnalyse = new JavaMethod(method);
            JavaCenter.link().registerMethod(methodAfterAnalyse);
            registerJSMethod(methodAfterAnalyse);
            return true;
        }
        return false;
    }

    private boolean extractJavaScriptInterfaceMethod(Method method) {
        if (android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.JELLY_BEAN_MR1) {
            JavascriptInterface annotation = method.getAnnotation(JavascriptInterface.class);
            if (annotation != null) {
                JavaMethod javaMethod = new JavaMethod(method);
                JavaCenter.link().registerMethod(javaMethod);
                registerJSMethod(javaMethod);
                return true;
            }
        }
        return false;
    }

    private void registerJSMethod(JavaMethod method) {
        mMethodNameArray.add(method.getName());
    }

    @CalledByNative
    public Object getMethodNameArray() {
        return mMethodNameArray;
    }

    @CalledByNative
    public Object exec(String methodName, Object[] args) {
        JavaMethod method = JavaCenter.link().findMethod(mReceiver, methodName, args);
        if (method != null) {
            return method.invoke(mReceiver, args);
        }
        Log.w("lynx", methodName + " Method not found");
        return null;
    }

    @CalledByNative
    private void destroy() {
        onDestroy();
    }

    public void onDestroy() {}

}
