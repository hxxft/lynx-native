package com.lynx.net;

import android.webkit.WebResourceResponse;

public interface RequestInterceptor {
    WebResourceResponse shouldInterceptRequest(String url);
}
