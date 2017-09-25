// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.net;

import android.content.Context;

import com.lynx.net.simple.SimpleRequestManager;
import com.lynx.net.simple.SimpleRequest;

/**
 * 通过此入口获得net相关实例
 */
public abstract class NetFactory {

    protected synchronized static NetRequestManager createRequestManager(Context context) {
        return new SimpleRequestManager(context);
    }

    protected static NetRequest createRequest(NetRequestBuilder builder) {
        NetRequest request = new SimpleRequest(builder);
        return request;
    }

}
