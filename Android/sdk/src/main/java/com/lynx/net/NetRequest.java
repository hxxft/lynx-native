// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.net;


import java.util.concurrent.ExecutionException;

/**
 * 进行get和post请求
 */
public abstract class NetRequest {
    private Object mExternalData;

    protected NetRequest(NetRequestBuilder builder) {
    }

    /**
     * 构建请求
     * @return
     */
    public final static NetRequestBuilder Builder() {
        return new NetRequestBuilder();
    }

    /**
     * 发送请求
     * @param response
     */
    public abstract void fetch(NetResponse response);

    /**
     * 同步请求
     * @return 请求结果
     * @throws InterruptedException
     * @throws ExecutionException
     */
    public abstract <T> T fetchSync() throws InterruptedException, ExecutionException;

    public abstract <T> T fetchFromCache();

    public abstract void cancel();

    public void setExternalData(Object data) {
        mExternalData = data;
    }

    public Object getExternalData() {
        return mExternalData;
    }

    public abstract NetResponse getNetResponse();
    public abstract NetRequestBuilder getNetRequestInfo();
}
