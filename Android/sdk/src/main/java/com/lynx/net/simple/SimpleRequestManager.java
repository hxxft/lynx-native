// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.net.simple;


import android.content.Context;

import com.lynx.net.NetRequest;
import com.lynx.net.NetRequestManager;
import com.lynx.net.NetWantedType;
import com.lynx.net.RequestInterceptor;
import com.lynx.net.external.ImageRequestManager;
import com.lynx.net.external.VolleyRequestManager;

import java.util.HashMap;
import java.util.Map;

public class SimpleRequestManager extends NetRequestManager {
    private Map<NetWantedType, NetRequestManager> mExecutorMap;

    public SimpleRequestManager(Context context) {
        mExecutorMap = new HashMap<>();
        mExecutorMap.put(NetWantedType.IMAGE, new ImageRequestManager(context));
        mExecutorMap.put(NetWantedType.COMMON, new VolleyRequestManager(context));
    }

    @Override
    public void cancelAllRequest() {
        for (NetRequestManager executor : mExecutorMap.values()) {
            executor.cancelAllRequest();
        }
    }

    @Override
    public void cancelRequest(NetRequest request) {
        mExecutorMap
                .get(request.getNetRequestInfo().getWantedType())
                .cancelRequest(request);
    }

    @Override
    public void start() {
        for (NetRequestManager executor : mExecutorMap.values()) {
            executor.start();
        }
    }

    @Override
    public void stop() {
        for (NetRequestManager executor : mExecutorMap.values()) {
            executor.stop();
        }
    }

    @Override
    public void fetch(NetRequest request) {
        mExecutorMap
                .get(request.getNetRequestInfo().getWantedType())
                .fetch(request);
    }

    @Override
    public <T> T  fetchSync(NetRequest request) {
        return mExecutorMap
                .get(request.getNetRequestInfo().getWantedType())
                .fetchSync(request);
    }

    @Override
    public <T> T fetchFromCache(NetRequest request) {
        return mExecutorMap
                .get(request.getNetRequestInfo().getWantedType())
                .fetchFromCache(request);
    }

    @Override
    public void addInterceptor(RequestInterceptor interceptor) {
        for (NetRequestManager executor : mExecutorMap.values()) {
            executor.addInterceptor(interceptor);
        }
    }

    @Override
    public void removeInterceptor(RequestInterceptor interceptor) {
        for (NetRequestManager executor : mExecutorMap.values()) {
            executor.removeInterceptor(interceptor);
        }
    }

}
