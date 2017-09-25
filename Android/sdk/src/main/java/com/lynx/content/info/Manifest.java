// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.content.info;

import com.lynx.resources.ResourceManager;

public class Manifest {

    App application;

    public App getApplication() {
        return application;
    }

    public void setApplication(App application) {
        this.application = application;
    }

    public static final String MATNIFEST_URI = ResourceManager.PROTOCOL_ASSETS + "manifest.json";
}
