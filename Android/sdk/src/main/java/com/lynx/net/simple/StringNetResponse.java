// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.net.simple;

import com.lynx.net.NetResponse;

public abstract class StringNetResponse implements NetResponse<String> {
    private String mUrl;

    public StringNetResponse(String url) {
        this.mUrl = url;
    }

    @Override
    public Class getType() {
        return String.class;
    }

    @Override
    public void onResponse(String response) {

    }

    @Override
    public void onFailure(String msg) {

    }
}
