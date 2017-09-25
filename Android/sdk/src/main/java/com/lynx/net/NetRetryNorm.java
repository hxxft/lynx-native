// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.net;

public class NetRetryNorm {
    private int mRetryTime;
    private int mRetryTimeoutMS;

    public NetRetryNorm() {
        this(1, 2500);
    }

    public NetRetryNorm(int retryTime, int retryTimeout) {
        this.mRetryTime = retryTime;
        this.mRetryTimeoutMS = retryTimeout;
    }

    public int getRetryTime() {
        return mRetryTime;
    }

    public int getRetryTimeoutMS() {
        return mRetryTimeoutMS;
    }
}
