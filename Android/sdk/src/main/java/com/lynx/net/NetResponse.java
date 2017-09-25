// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.net;

/**
 * 结果回调
 */
public interface NetResponse<T> {

    /**
     * 成功结果回调
     * @param response
     */
    void onResponse(T response);

    /**
     * 失败结果回调
     * @param msg
     */
    void onFailure(String msg);

    /**
     * 用来判断想要获得的请求结果的类型
     * @return
     */
    Class getType();
}
