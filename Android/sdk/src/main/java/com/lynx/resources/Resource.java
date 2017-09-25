// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.resources;

import android.graphics.BitmapFactory;

import com.lynx.utils.StringUtil;

import java.io.InputStream;

public class Resource {

    public enum Type {
        STRING,
        BITMAP
    }

    public static class Convector {

        public static Resource toResouce(InputStream in, Resource.Type type) {
            Object content = null;
            switch (type) {
                case STRING:
                    content = StringUtil.convertToString(in);
                    break;
                case BITMAP:
                    content = BitmapFactory.decodeStream(in);
                    break;
                default:
                    break;
            }
            if (content != null) {
                return new Resource(type, content);
            }
            return null;
        }

    }

    private Resource.Type mType;
    private Object mContent;

    public  Resource(Resource.Type type, Object content) {
        mType = type;
        mContent = content;
    }

    public Object getContent() {
        return mContent;
    }

}
