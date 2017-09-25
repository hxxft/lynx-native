// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.content.info;

import android.text.TextUtils;

import com.lynx.resources.ResourceManager;

public class Page {
    String name;
    String screenOrientation;

    // Example: App.vue
    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getScreenOrientation() {
        return screenOrientation;
    }

    public void setScreenOrientation(String screenOrientation) {
        this.screenOrientation = screenOrientation;
    }


    public static final String INDEX_HTML = "/index.html";

    private String mSimpleName;
    private String mHtmlPath;

    // Example: app
    public String getSimpleName() {
        if (TextUtils.isEmpty(mSimpleName) && !TextUtils.isEmpty(name)) {
            mSimpleName = name.substring(0, name.length() - 4).toLowerCase();
        }
        return mSimpleName;
    }

    public String getHtmlUri() {
        if (mHtmlPath == null) {
            mHtmlPath = ResourceManager.PROTOCOL_ASSETS + getSimpleName() + INDEX_HTML;
        }
        return mHtmlPath;
    }
}
