// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.modules;


import android.content.Context;
import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;

import com.lynx.core.LynxMethod;

public class PageNavigator extends LynxModule {
    private static final String DEFAULT_SCHEME = "lynx://";

    private Context mContext;

    public PageNavigator(Context context) {
        mContext = context;
    }

    @LynxMethod
    public void startNewPage(String page) {
        Bundle bundle = new Bundle();
        bundle.putString("page", page);
        Intent intent = new Intent(Intent.ACTION_VIEW, Uri.parse(DEFAULT_SCHEME + page));
        intent.putExtras(bundle);
        mContext.startActivity(intent);
    }

    @Override
    public String moduleName() {
        return "PageNavigator";
    }
}
