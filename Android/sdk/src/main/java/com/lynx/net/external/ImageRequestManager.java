// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.net.external;

import android.content.Context;
import android.graphics.Bitmap;
import android.text.TextUtils;
import android.widget.ImageView;

import com.lynx.net.NetRequest;
import com.lynx.net.NetRequestBuilder;
import com.lynx.net.NetRequestManager;
import com.lynx.net.NetResponse;
import com.lynx.net.RequestInterceptor;
import com.lynx.net.simple.ImageNetResponse;
import com.squareup.picasso.Callback;
import com.squareup.picasso.NetworkPolicy;
import com.squareup.picasso.Picasso;
import com.squareup.picasso.RequestCreator;
import com.squareup.picasso.Transformation;

import java.io.IOException;

public class ImageRequestManager extends NetRequestManager {

    private Picasso mPicasso;

    public ImageRequestManager(Context context) {
        mPicasso = Picasso.with(context);
    }

    @Override
    public void cancelAllRequest() {
    }

    @Override
    public void cancelRequest(NetRequest request) {
        ImageView imageView = null;

        Object externalData = request.getNetRequestInfo().getExternalData();

        if (externalData != null
                && externalData instanceof ImageRequestExternalData) {
            ImageRequestExternalData data = ((ImageRequestExternalData) externalData);
            imageView = data.getImageView();
        }

        Object tag = imageView == null ? this : imageView;
        if (imageView != null) {
            mPicasso.cancelRequest(imageView);
        } else {
            mPicasso.cancelTag(tag);
        }
    }

    @Override
    public void start() {

    }

    @Override
    public void stop() {
    }

    @Override
    public void fetch(NetRequest request) {
        NetRequestBuilder info = request.getNetRequestInfo();
        NetResponse response = request.getNetResponse();
        String url = info.getUrl();
        if (TextUtils.isEmpty(url)) {
            return;
        }
        ImageView imageView = null;
        ImageTransformation imageTransformation = null;

        Object externalData = info.getExternalData();

        if (externalData != null
                && externalData instanceof ImageRequestExternalData) {
            ImageRequestExternalData data = ((ImageRequestExternalData) externalData);
            imageView = data.getImageView();
            imageTransformation = data.getTransformation();
        }

        Object tag = imageView == null ? this : imageView;

        RequestCreator creator = mPicasso
                .load(url)
                .tag(tag)
                .noPlaceholder();

        if (imageTransformation != null) {
            creator.transform(new ImageRequestTransformation(imageTransformation));
        }

        LoadedListener loadedListener = new LoadedListener(mPicasso,
                url, (ImageNetResponse) response);

        if (imageView != null) {
            creator.into(imageView, loadedListener);
        } else {
            creator.fetch(loadedListener);
        }
    }

    @Override
    public <T> T  fetchSync(NetRequest request) {
        return null;
    }

    @Override
    public <T> T fetchFromCache(NetRequest request) {
        String url = request.getNetRequestInfo().getUrl();
        if (TextUtils.isEmpty(url)) {
            return null;
        }
        Bitmap result = null;
        try {
            result = mPicasso
                    .load(url)
                    .networkPolicy(NetworkPolicy.OFFLINE)
                    .get();
        } catch (IOException e) {
            e.printStackTrace();
        }
        return (T) result;
    }

    @Override
    public void addInterceptor(RequestInterceptor interceptor) {
    }

    @Override
    public void removeInterceptor(RequestInterceptor interceptor) {
    }

    static class LoadedListener implements Callback {
        private ImageNetResponse mResponse;
        private Picasso mPicasso;
        private String mUrl;

        public LoadedListener(Picasso picasso, String url, ImageNetResponse response) {
            mResponse = response;
            mPicasso = picasso;
            mUrl = url;
        }

        @Override
        public void onSuccess() {
            if (mResponse != null) {
                if (mResponse.getType() == Bitmap.class) {
                    Bitmap result = null;
                    try {
                        result = mPicasso
                                .load(mUrl)
                                .networkPolicy(NetworkPolicy.OFFLINE)
                                .get();
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                    mResponse.onResponse(result);
                } else {
                    mResponse.onResponse(null);
                }
            }
        }

        @Override
        public void onError() {
            if (mResponse != null) {
                mResponse.onFailure("Bitmap request error occurs internal!");
            }
        }
    }

    public static class ImageRequestTransformation implements Transformation {
        private ImageTransformation mImageTransformation;

        public ImageRequestTransformation(ImageTransformation transformation) {
            mImageTransformation = transformation;
        }

        @Override
        public Bitmap transform(Bitmap source) {
            return mImageTransformation.transform(source);
        }

        @Override
        public String key() {
            return mImageTransformation.key();
        }
    }
}
