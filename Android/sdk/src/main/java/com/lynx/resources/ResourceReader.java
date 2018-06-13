package com.lynx.resources;

import android.content.Context;

import com.google.gson.Gson;

import java.io.IOException;
import java.io.InputStream;
import java.util.ArrayDeque;
import java.util.Queue;

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
        path = PathFormat(path.replace(ResourceManager.kAssetModeApplicationLocation, ""));
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

    private String PathFormat(String path) {
        Queue<String> paths = new ArrayDeque<>();
        while(true) {
            int index = path.indexOf("/");
            if(index == -1) {
                paths.add(path);
                break;
            }
            String left = path.substring(0, index);
            String right = path.substring(index+1, path.length());
            if(left.equals("..")  && !paths.isEmpty()) {
                paths.poll();
            }else if(left != "/" && left != ".") {
                paths.add(left);
            }
            path = right;
        }

        String result = "";

        while(paths.size() > 1) {
            result += paths.poll();
            result += "/";
        }
        result += paths.poll();
        return result;
    }

}
