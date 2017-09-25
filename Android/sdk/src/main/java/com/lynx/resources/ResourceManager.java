// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.resources;

import android.content.Context;

import com.google.gson.Gson;

import java.io.IOException;
import java.io.InputStream;

public class ResourceManager {

    public static final String PROTOCOL_ASSETS = "Assets://";

    private static ResourceManager sResourceManager = null;

    private Context mContext;

    public static void init(Context context) {
        sResourceManager = new ResourceManager(context);
    }

    private ResourceManager(Context context) {
        mContext = context;
    }

    public String getString(String path) {
        return (String) getResource(path, Resource.Type.STRING).getContent();
    }

    public <T> T getObjectFromGson(String path, Class<T> clazz) {
        String gsonStr = getString(path);
        return new Gson().fromJson(gsonStr, clazz);
    }

    public Resource getResource(String path, Resource.Type type) {

        if (path.startsWith(PROTOCOL_ASSETS)) {
            return getResourceFromAssets(path, type);
        }

        return null;
    }

    private Resource getResourceFromAssets(String path, Resource.Type type) {

        path = path.replace(PROTOCOL_ASSETS, "");

        InputStream in = null;

        try {
            in = mContext.getAssets().open(path);
            return Resource.Convector.toResouce(in, type);
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            if (in != null) {
                try {
                    in.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
        return null;

    }

    public static ResourceManager instance() {
        if(sResourceManager == null) {
            throw new RuntimeException("ResourceManager has not been initialized!");
        }
        return sResourceManager;
    }

}
