// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.modules;


import android.content.Context;
import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;

import com.lynx.core.LynxRuntime;
import com.lynx.core.annotation.JSObject;
import com.lynx.core.annotation.JSMethod;
import com.lynx.modules.ext.JSComponent;

@JSObject
public class PageNavigator extends JSComponent {
    private static final String DEFAULT_SCHEME = "lynx://";

    private Context mContext;

    public PageNavigator(LynxRuntime runtime) {
        super(runtime);
        mContext = runtime.getContext();
    }

    @JSMethod
    public void startNewPage(String page) {
        Bundle bundle = new Bundle();
        bundle.putString("page", page);
        Intent intent = new Intent(Intent.ACTION_VIEW, Uri.parse(DEFAULT_SCHEME + page));
        intent.putExtras(bundle);
        mContext.startActivity(intent);
    }
}
