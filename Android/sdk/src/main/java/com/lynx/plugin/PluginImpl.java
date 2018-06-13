package com.lynx.plugin;

import com.lynx.base.CalledByNative;
import com.lynx.core.base.JavaCenter;
import com.lynx.core.base.JavaMethod;
import com.lynx.core.base.LynxObject;

import java.lang.reflect.Constructor;
import java.lang.reflect.InvocationTargetException;

public class PluginImpl extends LynxObject {

    protected String mPluginName = "";

    private long mNativePtr = 0;

    PluginImpl(String name, long ptr) {
        mPluginName = name;
        mNativePtr = ptr;
    }

    @CalledByNative
    public void exec(long clientId, Object[] args) {
        JavaMethod method = JavaCenter.link().findMethod(mReceiver, (String)args[1], args[3]);
        if (method != null) {
            method.invoke(mReceiver, clientId, args[2], args[3]);
        }
        return ;
    }

    @CalledByNative
    public void addEvent(String event) {

    }

    @CalledByNative
    public void removeEvent(String event) {

    }

    @CalledByNative
    static PluginImpl create(String name, long ptr) {
        PluginImpl plugin = null;
        String pluginName = "com.lynx.plugin." + name+"Plugin";
        try {
            Class clazz = Class.forName(pluginName);
            Constructor constructor = clazz.getConstructor(String.class, long.class);
            plugin = (PluginImpl)constructor.newInstance(name, ptr);
        } catch (ClassNotFoundException e) {
            e.printStackTrace();
        } catch (NoSuchMethodException e) {
            e.printStackTrace();
        } catch (IllegalAccessException e) {
            e.printStackTrace();
        } catch (InstantiationException e) {
            e.printStackTrace();
        } catch (InvocationTargetException e) {
            e.printStackTrace();
        }
        return plugin;
    }

    public void returnBack(long clientId, long methodId, boolean successed, Object[] args) {
        nativeReturnBack(mNativePtr, clientId, methodId, successed, args);
    }

    public void dispatchEvent(String event, Object[] args) {
        nativeDispatchEvent(mNativePtr, event, args);
    }

    private native void nativeReturnBack(long nativePtr, long clientId, long methodId, boolean successed, Object[] args);

    private native void nativeDispatchEvent(long nativePtr, String event, Object[] args);
}