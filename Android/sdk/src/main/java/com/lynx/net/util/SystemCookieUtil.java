// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.net.util;

import java.net.CookieHandler;
import java.net.CookieManager;
import java.net.HttpCookie;
import java.net.URI;
import java.util.List;

public class SystemCookieUtil {

    public synchronized static List<HttpCookie> getCookies(URI uri) {
        CookieManager cookieHandler = (CookieManager) CookieHandler.getDefault();
        if (cookieHandler == null) {
            return null;
        }
        java.net.CookieStore cookieStore = cookieHandler.getCookieStore();
        if (cookieStore != null) {
            return cookieStore.get(uri);
        }
        return null;
    }

    public static synchronized List<HttpCookie> getCookies() {
        CookieManager cookieHandler = (CookieManager) CookieHandler.getDefault();
        if (cookieHandler == null) {
            return null;
        }
        java.net.CookieStore cookieStore = cookieHandler.getCookieStore();
        if (cookieStore != null) {
            return cookieStore.getCookies();
        }
        return null;
    }

    public synchronized static List<URI> getURIs() {
        CookieManager cookieHandler = (CookieManager) CookieHandler.getDefault();
        if (cookieHandler == null) {
            return null;
        }
        java.net.CookieStore cookieStore = cookieHandler.getCookieStore();
        if (cookieStore != null) {
            return cookieStore.getURIs();
        }
        return null;
    }

}