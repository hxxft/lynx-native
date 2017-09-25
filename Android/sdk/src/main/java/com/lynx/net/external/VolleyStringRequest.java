// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.net.external;

import com.android.volley.AuthFailureError;
import com.android.volley.NetworkResponse;
import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.RequestFuture;
import com.android.volley.toolbox.StringRequest;
import com.lynx.net.NetResponse;
import com.lynx.net.util.HttpHeaderUtil;

import java.util.HashMap;
import java.util.Map;

public class VolleyStringRequest extends StringRequest{
    private Map<String, String> mHeaders;

    public VolleyStringRequest(int method, String url, final NetResponse response) {
        super(method, url, new ResponseListener(response), new ErrorListener(response));
    }

    public VolleyStringRequest(String url , RequestFuture<String> future) {
        super(url, future, future);
    }

    @Override
    protected Response<String> parseNetworkResponse(NetworkResponse response) {
        HttpHeaderUtil.resolveHeaders(response.headers, getUrl());
        return super.parseNetworkResponse(response);
    }

    public void setHeaders(Map<String, String> mHeaders) {
        this.mHeaders = mHeaders;
    }

    @Override
    public Map<String, String> getHeaders() throws AuthFailureError {
        if (mHeaders == null) {
            mHeaders = new HashMap<>();
        }
        return mHeaders;
    }

    static class ResponseListener implements Response.Listener<String>{
        private NetResponse mResponse;

        public ResponseListener(NetResponse response) {
            this.mResponse = response;
        }

        @Override
        public void onResponse(String s) {
            mResponse.onResponse(s);
        }
    }

    static class ErrorListener implements Response.ErrorListener{
        private NetResponse mResponse;

        public ErrorListener(NetResponse response) {
            this.mResponse = response;
        }

        @Override
        public void onErrorResponse(VolleyError volleyError) {
            if (volleyError != null) {
                mResponse.onFailure(volleyError.getMessage());
            }
        }
    }

}
