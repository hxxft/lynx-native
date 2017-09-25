// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.net;

import android.content.Context;

import java.util.HashMap;
import java.util.Map;

/**
 * 构建请求模板
 */
public class NetRequestBuilder {
    protected String url;
    protected String referer;
    protected String userAgent;
    protected Map<String, String> headers;
    protected boolean cache = true;
    protected boolean sync;
    protected Map<String, String> params;
    protected boolean resend;
    protected NetMethod method = NetMethod.GET;
    protected NetWantedType type = NetWantedType.COMMON;
    protected NetRetryNorm retryNorm;
    protected Object tag;
    private Object mExternalData;

    public NetRequestBuilder url(String url) {
        this.url = url;
        return this;
    }

    public NetRequestBuilder referer(String referer) {
        this.url = referer;
        return this;
    }

    public NetRequestBuilder userAgent(String userAgent) {
        this.userAgent = userAgent;
        return this;
    }

    public NetRequestBuilder method(NetMethod method) {
        this.method = method;
        return this;
    }

    public NetRequestBuilder addHeader(String key, String value) {
        if (this.headers == null) {
            this.headers = new HashMap<>();
        }
        this.headers.put(key, value);
        return this;
    }

    public NetRequestBuilder addHeaders(Map<String, String> headers) {
        if (this.headers == null) {
            this.headers = new HashMap<>();
        }
        this.headers.putAll(headers);
        return this;
    }

    public NetRequestBuilder headers(Map<String, String> headers) {
        this.headers = headers;
        return this;
    }

    public NetRequestBuilder shouldCache(boolean cache) {
        this.cache = cache;
        return this;
    }

    public NetRequestBuilder params(Map<String, String> params) {
        this.params = params;
        return this;
    }

    public NetRequestBuilder shouldResend(boolean resend) {
        this.resend = resend;
        return this;
    }

    public NetRequestBuilder retryNorm(NetRetryNorm norm) {
        this.retryNorm = norm;
        return this;
    }

    public NetRequestBuilder wantedType(NetWantedType type) {
        this.type = type;
        return this;
    }

    public NetRequestBuilder tag(Object tag) {
        this.tag = tag;
        return this;
    }

    public NetRequestBuilder externalData(Object data) {
        mExternalData = data;
        return this;
    }

//    public NetRequest build(JSRuntime runtime) {
//        return NetFactory.createRequest(runtime, this);
//    }
    public NetRequest build() {
        return NetFactory.createRequest(this);
    }

    public String getUrl() {
        return url;
    }

    public String getReferer() {
        return referer;
    }

    public String getUserAgent() {
        return userAgent;
    }

    public Map<String, String> getHeaders() {
        return headers;
    }

    public boolean shouldCache() {
        return cache;
    }

    public Map<String, String> getParams() {
        return params;
    }

    public boolean shouldResend() {
        return resend;
    }

    public NetMethod getMethod() {
        return method;
    }

    public NetRetryNorm getRetryNorm() {
        return retryNorm;
    }

    public NetWantedType getWantedType() {
        return type;
    }

    public Object getTag() {
        return tag;
    }

    public Object getExternalData() {
        return mExternalData;
    }
}
