// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.content.info;

import android.text.TextUtils;

import com.lynx.resources.ResourceManager;

public class Page {
    private String name;
    private String screenOrientation;

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

    private String mPageName;

    // Example: app
    public String getPageName() {
        if (TextUtils.isEmpty(mPageName) && !TextUtils.isEmpty(name)) {
            mPageName = name.substring(0, name.length() - 4).toLowerCase();
        }
        return mPageName;
    }

    public String getLayoutResourceURL() {
        return ResourceManager.ASSET_PROTOCOL + getPageName() + INDEX_HTML;
    }

    public String getResourceURL() {
        return ResourceManager.ASSET_PROTOCOL + getPageName() + "/";
    }
}
