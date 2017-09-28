// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.resources;

import android.content.Context;

public class ResourceManager {

    private static ResourceManager sResourceManager = null;

    private ResourceReader mReader;

    public static String ASSET_PROTOCOL = "Asset://";
    public static String DATA_PROTOCOL = "Data://";
    public static String DEBUG_PROTOCOL = "Debug://";

    public static String kAssetModeApplicationLocation = "file:///android_asset/";
    public static String kDataModeApplicationLocation = "file://";
    public static String kDebugModeApplicationLocation = "http://127.0.0.1:3000/";

    private static String FILE_PROTOCOL = "file://";

    public static String toRealURL(String resource) {
        String resourceURL = resource;
        if(resource.startsWith(ASSET_PROTOCOL)) {
            resourceURL =  kAssetModeApplicationLocation + resource.replace(ASSET_PROTOCOL, "");
        }else if(resource.startsWith(DATA_PROTOCOL)) {
            resourceURL =  kDataModeApplicationLocation + resource.replace(DATA_PROTOCOL, "");
        }else if(resource.startsWith(DEBUG_PROTOCOL)) {
            resourceURL =  kDebugModeApplicationLocation + resource.replace(DEBUG_PROTOCOL, "");
        }

        return resourceURL;
    }

    public static boolean isLocalResource(String readResourceURL) {
        if(readResourceURL.startsWith(FILE_PROTOCOL)) {
            return true;
        }
        return false;
    }

    public ResourceReader reader() {
        return mReader;
    }

    public static void init(Context context) {
        sResourceManager = new ResourceManager(context);
    }

    private ResourceManager(Context context) {
        mReader = new ResourceReader(context);
    }



    public static ResourceManager instance() {
        if(sResourceManager == null) {
            throw new RuntimeException("ResourceManager has not been initialized!");
        }
        return sResourceManager;
    }

}
