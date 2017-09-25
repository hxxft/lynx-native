// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.net.simple;

import com.lynx.net.NetResponse;

public class ImageNetResponse<T> implements NetResponse<T> {

    @Override
    public void onResponse(T response) {

    }

    @Override
    public void onFailure(String msg) {

    }

    @Override
    public Class getType() {
        return null;
    }
}
