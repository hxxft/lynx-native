// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.ui.image;

import android.content.Context;
import android.graphics.Bitmap;
import android.text.TextUtils;
import android.widget.ImageView;

import com.lynx.base.Position;
import com.lynx.base.Style;
import com.lynx.core.impl.RenderObjectImpl;
import com.lynx.net.NetRequest;
import com.lynx.net.NetWantedType;
import com.lynx.net.external.ImageRequestExternalData;
import com.lynx.net.simple.ImageNetResponse;
import com.lynx.ui.LynxUI;
import com.lynx.ui.recycler.HiSpeedStopLoadItem;

public class LynxUIImage
        extends LynxUI<AndroidImage>
        implements HiSpeedStopLoadItem {

    private static final String IMAGE_SRC = "src";
    private static final String PLACE_HOLDER = "placeholder";

    private String mCurUrl;
    // Do not load image when high speed
    private boolean mShouldStopImageLoadWhenHiSpeed = false;
    // For update style when url is decoded as a animated image
    private boolean isAnimatedImage = false;
    // Decide whether load image immediately
    private boolean mIsAttachedToWindow = false;
    // Helping make sure is a image has been set.
    private boolean mHasImageSet = true;
    private boolean mHasBindData;
    private LoadedInfo mLoadedInfo;
    private PlaceHolderLoadedListener mPlaceHolderLoadedListener;
    // For detecting whether the same url should be set more than once.
    private LynxUIImageTransformation mCurImageTransformation;
    private NetRequest mImageRequest;

    public LynxUIImage(Context context, RenderObjectImpl impl) {
        super(context, impl);
    }

    @Override
    protected AndroidImage createView(Context context) {
        return new AndroidImage(context, this);
    }

    @Override
    public void bindData(RenderObjectImpl impl) {
        super.bindData(impl);
        mHasBindData = true;
        updateImageSrc();
    }

    @Override
    public void unbindData() {
        super.unbindData();
        // Reset image loaded state
        if (mLoadedInfo != null) {
            mLoadedInfo.success = false;
        }
        mCurUrl = null;
        mHasImageSet = false;
        // Cancel request to keep the view cleaning
        cancelImageRequest();
        mHasBindData = false;
    }

    void updateAnimatedStyle() {
        isAnimatedImage = true;
        setScale();
        setPadding();
    }


    @Override
    public void setAttribute(String key, String value) {
        super.setAttribute(key, value);
        if (key.equals(IMAGE_SRC)) {
            setImageUrl(mRenderObjectImpl.getAttribute(IMAGE_SRC));
        }
        else if (key.equals(PLACE_HOLDER)) {
            setPlaceHolder();
        }
    }

    private void updateImageSrc() {

        // First set place holder to prevent a long net work causing a blank view.
        setPlaceHolder();

        // Second load image.
        setImageUrl(mRenderObjectImpl.getAttribute(IMAGE_SRC));
    }

    private void setPlaceHolder() {


        if (!mIsAttachedToWindow
                || !mHasBindData
                || mLoadedInfo == null
                || mLoadedInfo.success) {
            return;
        }

        if (mRenderObjectImpl == null) {
            mView.setImageBitmap(null);
            return;
        }

        Bitmap bitmap = null;

        if (!(mRenderObjectImpl.getAttribute(PLACE_HOLDER) == null)) {
            if (mPlaceHolderLoadedListener == null) {
                mPlaceHolderLoadedListener = new PlaceHolderLoadedListener();
            }
            bitmap = ImagePlaceHolder.instance()
                    .get(mRenderObjectImpl.getAttribute(PLACE_HOLDER),
                        mPlaceHolderLoadedListener);
        }

        if (bitmap != null) {
            mView.setImageBitmap(bitmap);
        } else {
            mView.setImageBitmap(null);
        }
    }

    public void setImageUrl(final String url) {

        if (mShouldStopImageLoadWhenHiSpeed
                || !mHasBindData) {
            return;
        }

        if (TextUtils.isEmpty(url)) {

            mCurImageTransformation = null;

            // Cancel previous request
            cancelImageRequest();

            mCurUrl = url;
            mHasImageSet = false;
            isAnimatedImage = false;

        } else {

            if (mLoadedInfo == null) {
                mLoadedInfo = new LoadedInfo();
            }

            LynxUIImageTransformation newImageTransformation = new LynxUIImageTransformation(getRenderObjectImpl());

            if ((mCurUrl != null && mCurUrl.equals(url))
                    && newImageTransformation.key() != null
                    && mCurImageTransformation != null
                    && newImageTransformation.key().equals(mCurImageTransformation.key())) {

            } else {

                mCurImageTransformation = newImageTransformation;

                mCurUrl = url;
                mHasImageSet = true;
                isAnimatedImage = false;

                if (mIsAttachedToWindow) {
                    triggerLoadProcess();
                }
            }
        }
    }

    // When view on attach to window, loading image immediately
    protected void onAttachedToWindow() {
        mIsAttachedToWindow = true;

        // 1. Set place holder
        setPlaceHolder();
        // 2. Trigger image loading
        triggerLoadProcess();
    }

    // When view detaches from window, clear image
    protected void onDetachedFromWindow() {
        mIsAttachedToWindow = false;

        // Cancel previous request before create a new request
        cancelImageRequest();
        // Release bitmap outside screen
        mView.setImageBitmap(null);
    }

    private void triggerLoadProcess() {

        if (!mHasImageSet || mCurImageTransformation == null || TextUtils.isEmpty(mCurUrl)) {
            return;
        }

        // Cancel previous request before create a new request
        cancelImageRequest();

        ImageRequestExternalData data = new ImageRequestExternalData(mView, mCurImageTransformation);

        mImageRequest = NetRequest.Builder()
                .wantedType(NetWantedType.IMAGE)
                .url(mCurUrl)
                .externalData(data)
                .build();

        mImageRequest.fetch(mLoadedInfo);
    }

    @Override
    public void updateStyle(Style style) {
        setScale();
        setPadding();
        super.updateStyle(style);
        // Setting image after updating style as image will change with style changing.
        updateImageSrc();
    }

    @Override
    public void setPosition(Position position) {
        super.setPosition(position);
        // Setting image after updating position as image will change with position changing.
        updateImageSrc();
    }

    private void setPadding() {
        int borderWidth = (int) mRenderObjectImpl.getStyle().mBorderWidth;
        if (isAnimatedImage
                || (borderWidth > 0
                    && mRenderObjectImpl.getStyle().mBorderRadius == 0)) {
            mView.setPadding(borderWidth, borderWidth, borderWidth, borderWidth);
        } else {
            mView.setPadding(0, 0, 0, 0);
        }
    }

    private void setScale(){

        if (mRenderObjectImpl != null
                && mRenderObjectImpl.getStyle().mBorderRadius > 0
                && !isAnimatedImage) {
            // With radius setting, just set center type
            // as all the style will be do in ImageTransformation.
            mView.setScaleType(ImageView.ScaleType.CENTER);
            return;
        }

        switch (mRenderObjectImpl.getStyle().mObjectFit){
            case Style.CSSIMAGE_OBJECT_FIT_FILL:
                mView.setScaleType(ImageView.ScaleType.FIT_XY);
                break;
            case Style.CSSIMAGE_OBJECT_FIT_CONTAIN:
                mView.setScaleType(ImageView.ScaleType.FIT_CENTER);
                break;
            case Style.CSSIMAGE_OBJECT_FIT_COVER:
                mView.setScaleType(ImageView.ScaleType.CENTER_CROP);
                break;
            default:break;
        }
    }

    private void cancelImageRequest() {
        if (mImageRequest != null) {
            mImageRequest.cancel();
        }
    }

    @Override
    public void stopLoadResWhenViewInHiSpeed() {
        mShouldStopImageLoadWhenHiSpeed = true;
    }

    @Override
    public void allowLoadResWhenViewInHiSpeed() {
        mShouldStopImageLoadWhenHiSpeed = false;
    }

    @Override
    public void reloadResWhenSuitable() {
        mShouldStopImageLoadWhenHiSpeed = false;
        updateImageSrc();
    }

    private class LoadedInfo extends ImageNetResponse<Void> {
        boolean success = false;

        @Override
        public void onResponse(Void response) {
            success = true;
        }

        @Override
        public void onFailure(String msg) {
            success = false;
            setPlaceHolder();
        }
    }

    private class PlaceHolderLoadedListener extends ImagePlaceHolder.LoadedListener {

        @Override
        public void onResponse(Void response) {
            setPlaceHolder();
        }

        @Override
        public void onFailure(String msg) {

        }
    }

}
