// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.net;

import android.content.Context;

import com.lynx.net.cookie.LynxCookieStore;

/**
 * 负责增加或删除request
 */
public abstract class NetRequestManager {
    private static NetRequestManager sManager;

    public static synchronized NetRequestManager init(Context context) {
        if (sManager != null) {
            throw new RuntimeException("NetRequestManager has been initialized!");
        }
        sManager = NetFactory.createRequestManager(context);
        return sManager;
    }

    public static NetRequestManager instance() {
        if (sManager == null) {
            throw new RuntimeException("NetRequestManager has not been initialized!");
        }
        return sManager;
    }

    /**
     * 删除所有待请求
     */
    public abstract void cancelAllRequest();

    /**
     * 删除单个请求
     * @param request
     */
    public abstract void cancelRequest(NetRequest request);

    /**
     * 开启请求,默认开启,支持在stop后进行start使用
     */
    public abstract void start();

    /**
     * 停止请求
     */
    public abstract void stop();

    public abstract void fetch(NetRequest request);
    public abstract <T> T fetchSync(NetRequest request);
    public abstract <T> T fetchFromCache(NetRequest request);

    public abstract void addInterceptor(RequestInterceptor interceptor);
    public abstract void removeInterceptor(RequestInterceptor interceptor);

}
