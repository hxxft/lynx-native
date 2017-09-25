// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.net.util;

import android.text.TextUtils;

import com.lynx.net.NetRequestBuilder;
import com.lynx.net.cookie.LynxCookieStore;

import java.net.HttpCookie;
import java.net.URI;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class HttpHeaderUtil {

    /**
     * 模拟webview请求头
     * @return
     */
    public static synchronized Map<String, String> getNeededHeaders(NetRequestBuilder builder) {
        String pageUrl = builder.getReferer();
        Map<String, String>  params = new HashMap<>();
        params.put("User-Agent", builder.getUserAgent());
        if(pageUrl != null && pageUrl.length()>5){
            params.put("Referer", pageUrl);
        }
        URI uri = URI.create(builder.getUrl());
        params.put("Host", uri.getHost());
        params.put("Path", uri.getPath());

        java.net.CookieStore cookieStore = LynxCookieStore.getDefault();
        List<HttpCookie> cookies = cookieStore.get(uri);
        if(cookies != null && cookies.size()>0){
            params.put("Cookie", TextUtils.join("; ", cookies));
        }
        params.put("Accept", "*/*");

        return params;
    }

    /**
     * 解析http请求头,进行cookie存储、字符编码等操作
     * @param headers
     * @param url
     */
    public static synchronized void resolveHeaders(Map<String, String> headers, String url) {
        // 增加字符编码，默认为 UTF-8，如果 Content-Type 已经有，则不进行字符编码增加
        // 由于 Volley 从 Cache 回来的是 HashMap ，对大小写敏感，需要增加小写判断。
        // 从网络回来的是 TreeMap ，对大小写不敏感
        String contentType = headers.get("Content-Type"); // 大写是针对于 Volley 的字符编码
        final String charset = "charset=UTF-8";
        if (contentType == null) {
            String otherContentType = headers.get("content-type");
            if (otherContentType != null) {
                contentType = otherContentType + ";" + charset;
            }
        } else if (contentType != null && !contentType.contains("charset")) {
            contentType += ";" + charset;
        }
        // Volley 会根据该字段获取 charset 进行编码
        headers.put("Content-Type",contentType);

        // 保存 Cookie
        String cookieStr = headers.get("Set-Cookie");
        List<String> setCookieArray = new ArrayList<>();
        String setCookieSize = headers.get("Set-Cookie__size");
        if(setCookieSize != null){
            int cookieSize = Integer.parseInt(setCookieSize);
            for(int i = 0;i<cookieSize;i++){
                setCookieArray.add(headers.get("Set-Cookie__"+i));
            }
        }else if(cookieStr != null){
            setCookieArray.add(cookieStr);
        }
        if(setCookieArray.size() > 0){
            java.net.CookieStore cookieStore = LynxCookieStore.getDefault();
            for(String cookieItemString:setCookieArray){
                List<HttpCookie> httpCookies = HttpCookie.parse("Set-Cookie:"+cookieItemString);
                cookieStore.add(URI.create(url), httpCookies.get(0));
            }
        }

    }
}