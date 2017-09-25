// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.net.simple;

import android.support.annotation.Nullable;

import com.lynx.net.NetRequest;
import com.lynx.net.NetRequestBuilder;
import com.lynx.net.NetRequestManager;
import com.lynx.net.NetResponse;

import java.util.concurrent.ExecutionException;

public class SimpleRequest extends NetRequest {
    private final NetRequestBuilder mInfo;
    private NetResponse mResponse;

    public SimpleRequest(NetRequestBuilder builder) {
        super(builder);
        mInfo = builder;
    }

    @Override
    public void fetch(NetResponse response) {
        mResponse = response;
        NetRequestManager.instance().fetch(this);
    }

    @Override
    public @Nullable <T> T fetchSync() throws InterruptedException, ExecutionException{
        return (T) NetRequestManager.instance().fetchSync(this);
    }

    @Override
    public <T> T fetchFromCache() {
        return NetRequestManager.instance().fetchFromCache(this);
    }

    @Override
    public void cancel() {
        NetRequestManager.instance().cancelRequest(this);
    }

    @Override
    public NetResponse getNetResponse() {
        return mResponse;
    }

    @Override
    public NetRequestBuilder getNetRequestInfo() {
        return mInfo;
    }

}
