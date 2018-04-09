// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.net.external;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.drawable.Drawable;
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
import com.squareup.picasso.Target;
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

        Object tag = imageView == null ? request : imageView;
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
        RequestCreator requestCreator = requestCreator(mPicasso, request);
        if (requestCreator == null) return;

        ImageView imageView = null;
        Object externalData = request.getNetRequestInfo().getExternalData();
        if (externalData != null
                && externalData instanceof ImageRequestExternalData) {
            imageView = ((ImageRequestExternalData) externalData).getImageView();
        }

        LoadedListener loadedListener = new LoadedListener(mPicasso, request,
                request.getNetResponse());

        if (imageView != null) {
            requestCreator.into(imageView, loadedListener);
        } else {
            requestCreator.fetch(loadedListener);
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
        return fetchFromCache(mPicasso, request);
    }

    private static  <T> T fetchFromCache(Picasso picasso, NetRequest request) {
        CacheDetector detector = new CacheDetector();
        RequestCreator creator = requestCreator(picasso, request);
        if (creator != null) {
            // Async action
            creator.networkPolicy(NetworkPolicy.OFFLINE).into(detector);
        }
        return (T) detector.bitmap;
    }

    private static RequestCreator requestCreator(Picasso picasso, NetRequest request) {
        NetRequestBuilder info = request.getNetRequestInfo();
        String url = info.getUrl();
        if (TextUtils.isEmpty(url)) {
            return null;
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

        Object tag = imageView == null ? request : imageView;

        RequestCreator creator = picasso
                .load(url)
                .tag(tag)
                .noPlaceholder();

        if (imageTransformation != null) {
            creator.transform(new ImageRequestTransformation(imageTransformation));
        }
        return creator;
    }

    @Override
    public void addInterceptor(RequestInterceptor interceptor) {
    }

    @Override
    public void removeInterceptor(RequestInterceptor interceptor) {
    }

    static class LoadedListener implements Callback {
        private NetResponse mResponse;
        private Picasso mPicasso;
        private NetRequest mNetRequest;

        public LoadedListener(Picasso picasso, NetRequest request, NetResponse response) {
            mResponse = response;
            mPicasso = picasso;
            mNetRequest = request;
        }

        @Override
        public void onSuccess() {
            if (mResponse != null) {
                if (mResponse.getType() == Bitmap.class) {
                    Bitmap result = fetchFromCache(mPicasso, mNetRequest);
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

    private static class CacheDetector implements Target {
        public Bitmap bitmap;

        @Override
        public void onBitmapLoaded(Bitmap bitmap, Picasso.LoadedFrom from) {
            this.bitmap = bitmap;
        }

        @Override
        public void onBitmapFailed(Drawable errorDrawable) {
        }

        @Override
        public void onPrepareLoad(Drawable placeHolderDrawable) {
        }
    }
}
