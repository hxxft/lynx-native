// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.ui.image;

import android.graphics.Bitmap;
import android.text.TextUtils;

import com.lynx.net.NetRequest;
import com.lynx.net.NetWantedType;
import com.lynx.net.simple.ImageNetResponse;

public class ImagePlaceHolder {

    private static ImagePlaceHolder sPlaceHolder;

    private ImagePlaceHolder() {
    }

    public static synchronized ImagePlaceHolder instance() {
        if (sPlaceHolder == null) {
            sPlaceHolder = new ImagePlaceHolder();
        }
        return sPlaceHolder;
    }

    public Bitmap get(String url) {
        return get(url, null);
    }

    public Bitmap get(String url, LoadedListener listener) {
        if (TextUtils.isEmpty(url)) {
            return null;
        }
        NetRequest request = NetRequest.Builder()
                .wantedType(NetWantedType.IMAGE)
                .url(url)
                .build();

        Bitmap cache = request.fetchFromCache();
        if (cache != null) {
            return cache;
        } else {
            request.fetch(listener);
            return null;
        }
    }

    public static class LoadedListener extends ImageNetResponse<Void> {
    }
}
