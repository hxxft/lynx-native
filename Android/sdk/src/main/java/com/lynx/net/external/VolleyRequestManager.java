// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.net.external;

import android.content.Context;
import android.util.Log;
import android.webkit.WebResourceResponse;

import com.android.volley.DefaultRetryPolicy;
import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.toolbox.Volley;
import com.lynx.net.NetMethod;
import com.lynx.net.NetRequest;
import com.lynx.net.NetRequestBuilder;
import com.lynx.net.NetRequestManager;
import com.lynx.net.NetResponse;
import com.lynx.net.RequestInterceptor;
import com.lynx.net.util.HttpHeaderUtil;
import com.lynx.net.util.URLUtil;
import com.lynx.utils.StringUtil;

import java.util.ArrayList;
import java.util.List;

public class VolleyRequestManager extends NetRequestManager {
    private RequestQueue mRequestQueue;
    private List<RequestInterceptor> mInterceptorList;

    public VolleyRequestManager(Context context) {
        mRequestQueue = Volley.newRequestQueue(context);
        mInterceptorList = new ArrayList<>();
    }

    @Override
    public void cancelAllRequest() {
        this.stop();
        this.start();
    }

    @Override
    public void cancelRequest(NetRequest request) {
        mRequestQueue.cancelAll(request);
    }

    /**
     * 开启请求,默认开启,支持在stop后进行start使用
     */
    @Override
    public void start() {
        mRequestQueue.start();
    }

    @Override
    public void stop() {
        mRequestQueue.stop();
    }

    @Override
    public void fetch(NetRequest request) {
        NetRequestBuilder info = request.getNetRequestInfo();
        if (info.getMethod() == NetMethod.GET) {
            get(request);
        } else if (info.getMethod() == NetMethod.POST) {
            post(request);
        } else {
            Log.w("SimpleRequest", "did not give a specified request method!");
        }
    }

    protected void get(NetRequest request) {
        NetRequestBuilder info = request.getNetRequestInfo();
        NetResponse response = request.getNetResponse();
        // 判断是否有离线资源，有则直接获取离线资源返回
        for (int i = mInterceptorList.size() - 1; i >= 0; --i) {
            WebResourceResponse webResourceResponse =
                    mInterceptorList.get(i).shouldInterceptRequest(info.getUrl());
            if (webResourceResponse != null
                    && webResourceResponse.getMimeType().startsWith("text/")
                    && request.getNetResponse().getType() == String.class) {
                String result = StringUtil.convertToString(webResourceResponse.getData(),
                        webResourceResponse.getEncoding());
                request.getNetResponse().onResponse(result);
                return;
            }
        }

        // 进行 url 中文编码操作
        String encodedUrl = URLUtil.encoder(info.getUrl());
        // 若无离线资源，则进行网络请求
        VolleyStringRequest volleyRequest = new VolleyStringRequest(
                Request.Method.GET, encodedUrl, response);
        volleyRequest.setShouldCache(info.shouldCache());
        volleyRequest.setTag(request);
        volleyRequest.setHeaders(HttpHeaderUtil.getNeededHeaders(info));
        if (info.getRetryNorm() != null) {
            volleyRequest.setRetryPolicy(new DefaultRetryPolicy(
                    info.getRetryNorm().getRetryTimeoutMS(),
                    info.getRetryNorm().getRetryTime(),
                    DefaultRetryPolicy.DEFAULT_BACKOFF_MULT));
        }

        mRequestQueue.add(volleyRequest);
    }

    protected void post(NetRequest request) {

    }

    @Override
    public <T> T  fetchSync(NetRequest request) {
        return (T) "";
    }

    @Override
    public <T> T fetchFromCache(NetRequest request) {
        return null;
    }

    @Override
    public void addInterceptor(RequestInterceptor interceptor) {
        mInterceptorList.add(interceptor);
    }

    @Override
    public void removeInterceptor(RequestInterceptor interceptor) {
        mInterceptorList.remove(interceptor);
    }

}
