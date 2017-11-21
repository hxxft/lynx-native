// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.base;

import android.support.annotation.Keep;

import com.lynx.net.NetMethod;
import com.lynx.net.NetRequest;
import com.lynx.net.NetRetryNorm;
import com.lynx.net.simple.StringNetResponse;
import com.lynx.resources.ResourceManager;

public class URLRequest {
    private NetRequest mRequest;
    private long mDelegate;

    public URLRequest(final String url, long delegate) {
        mDelegate = delegate;
        mRequest = NetRequest.Builder()
                .method(NetMethod.GET)
                .url(url)
                .retryNorm(new NetRetryNorm(4, 1000))  // 重试4次
                .build();
    }

    @CalledByNative
    static URLRequest create(final String url, long delegate) {
        return new URLRequest(url, delegate);
    }

    @Keep
    @CalledByNative
    public void fetch() {
       final String url = mRequest.getNetRequestInfo().getUrl();

        mRequest.fetch(new StringNetResponse(url) {
            @Override
            public void onResponse(String response) {
                // Called on UI thread
                if (mDelegate != 0 && !response.isEmpty()) {
                    nativeOnSuccess(mDelegate, url, response);
                }
            }

            @Override
            public void onFailure(String error) {
                // Called on UI thread
                if (mDelegate != 0 && error != null && !error.isEmpty()) {
                    nativeOnFailed(mDelegate, url, error);
                }
            }

        });
    }

    @CalledByNative
    public void cancel() {
        mDelegate = 0;
        mRequest.cancel();
    }

    @Keep
    public native void nativeOnSuccess(long ptr, String url, String response);

    @Keep
    public native void nativeOnFailed(long ptr, String url, String error);

}
