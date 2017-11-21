package com.lynx.resources;

import android.content.Context;

import com.google.gson.Gson;

import java.io.IOException;
import java.io.InputStream;

/**
 * Created by dli on 2017/9/28.
 */

public class ResourceReader {

    private Context mContext;

    ResourceReader(Context context) {
        this.mContext = context;
    }

    public <T> T readResourceAsJSON(String url, Class<T> clazz) {
        String gsonStr = readResourceAsString(url);
        return new Gson().fromJson(gsonStr, clazz);
    }

    public String readResourceAsString(String url) {
        return (String)getResourceFromAssets(url, Resource.Type.STRING).getContent();
    }

    private Resource getResourceFromAssets(String path, Resource.Type type) {
        path = path.replace(ResourceManager.kAssetModeApplicationLocation, "");
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

}
