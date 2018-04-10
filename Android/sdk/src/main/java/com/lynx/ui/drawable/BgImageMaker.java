// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.ui.drawable;

import android.graphics.Bitmap;
import android.graphics.BitmapShader;
import android.graphics.Canvas;
import android.graphics.Matrix;
import android.graphics.Paint;
import android.graphics.Rect;
import android.graphics.Shader;
import android.support.annotation.NonNull;
import android.text.TextUtils;

import com.lynx.base.Constants;
import com.lynx.base.Position;
import com.lynx.base.Style;
import com.lynx.net.NetRequest;
import com.lynx.net.NetResponse;
import com.lynx.net.NetWantedType;
import com.lynx.net.external.ImageRequestExternalData;

public class BgImageMaker implements IMaker, NetResponse<Bitmap> {

    private String mUrl;
    private Rect mBounds;
    private Paint mPaint;
    private Bitmap mBitmap;
    private IControl mControl;
    private Rect mImageDrawRect;
    private NetRequest mNetRequest;

    private boolean mDrawEnable;
    private boolean mRequireReload;

    private int mRepeatMode;
    private int mPosX, mPosY;
    private int mImgWidth = Constants.UNDEFINED;
    private int mImgHeight = Constants.UNDEFINED;

    public BgImageMaker(IControl control) {
        mControl = control;
        mBounds = new Rect();
        mImageDrawRect = new Rect();
        mPaint = new Paint(Paint.ANTI_ALIAS_FLAG);
    }

    @Override
    public void updateBounds(Position bounds) {
        mBounds.set(0, 0, bounds.getWidth(), bounds.getHeight());

        if (mRequireReload) {
            loadImage();
            mRequireReload = false;
        }
    }

    @Override
    public void updateStyle(Style style) {
        boolean requireInvalidate = false;

        if (!TextUtils.isEmpty(style.mBackgroundImage) && !style.mBackgroundImage.equals(mUrl)) {
            mUrl = style.mBackgroundImage;
            if (mNetRequest != null && mBitmap != null) {
                mNetRequest.cancel();
            }
            if (mBitmap != null) {
                mBitmap.recycle();
                mBitmap = null;
            }
            mNetRequest = null;
            mRequireReload = true;
        }

        if (style.mBackgroundWidth != mImgWidth || style.mBackgroundHeight != mImgHeight) {
            // If wanted width and height change, image should be re-scale.
            mImgWidth = style.mBackgroundWidth;
            mImgHeight = style.mBackgroundHeight;
            mRequireReload = true;
        }

        if (mRepeatMode != style.mBackgroundRepeat || mPosX != style.mBackgroundPositionX
                || mPosY != style.mBackgroundPositionY) {
            mRepeatMode = style.mBackgroundRepeat;
            setPaintShader();
            mPosX = style.mBackgroundPositionX;
            mPosY = style.mBackgroundPositionY;
            requireInvalidate = true;
        }

        // Wait until the view has size.
        requireInvalidate = requireInvalidate && !mBounds.isEmpty() && mBitmap != null;

        if (mRequireReload && !mBounds.isEmpty()) {
            loadImage();
            mRequireReload = false;
        } else if (requireInvalidate) {
            mControl.invalidate();
        }
    }

    private void setPaintShader() {
        if (mBitmap != null) {
            mPaint.setShader(new BitmapShader(mBitmap,
                    Shader.TileMode.REPEAT, Shader.TileMode.REPEAT));
        }
    }

    private void loadImage() {
        mDrawEnable = false;
        mNetRequest = NetRequest.Builder()
                .wantedType(NetWantedType.IMAGE)
                .url(mUrl)
                .externalData(new ImageRequestExternalData(
                        new InnerTransformation(mImgWidth, mImgHeight)))
                .build();
        mNetRequest.fetch(this);
    }

    @Override
    public void draw(@NonNull Canvas canvas) {
        if (!mDrawEnable) return;
        canvas.translate(mPosX, mPosY);
        switch (mRepeatMode) {
            case Style.CSS_BACKGROUND_REPEAT:
                mImageDrawRect.set(-mPosX, -mPosY, mBounds.width() - mPosX,
                        mBounds.height() - mPosY);
                break;
            case Style.CSS_BACKGROUND_REPEAT_Y:
                mImageDrawRect.set(0, -mPosY, mBitmap.getWidth(),
                        mBounds.height() - mPosY);
                break;
            case Style.CSS_BACKGROUND_REPEAT_X:
                mImageDrawRect.set(-mPosX, 0, mBounds.width() - mPosX,
                        mBitmap.getHeight());
                break;
            case Style.CSS_BACKGROUND_NO_REPEAT:
                mImageDrawRect.set(0, 0, mBitmap.getWidth(), mBitmap.getHeight());
                break;
        }
        canvas.drawRect(mImageDrawRect, mPaint);
        canvas.translate(-mPosX, -mPosY);
    }

    @Override
    public void onResponse(Bitmap response) {
        mBitmap = response;
        setPaintShader();
        mDrawEnable = true;
        mControl.invalidate();
    }

    @Override
    public void onFailure(String msg) {
        mDrawEnable = false;
        mBitmap = null;
    }

    @Override
    public Class getType() {
        return Bitmap.class;
    }

    private static class InnerTransformation implements com.lynx.net.external.ImageTransformation {

        private static Matrix sScaleMatrix = new Matrix();

        private String mKey;
        private int mImgWidth;
        private int mImgHeight;

        InnerTransformation(int imgWidth, int imgHeight) {
            mImgWidth = imgWidth;
            mImgHeight = imgHeight;

            mKey = mImgWidth + ";" + mImgHeight + ";";
        }

        @Override
        public Bitmap transform(Bitmap source) {

            Bitmap result;

            float sx = 1;
            float sy = 1;
            if (mImgWidth != Constants.UNDEFINED && mImgHeight != Constants.UNDEFINED) {
                sx = mImgWidth / (float) source.getWidth();
                sy = mImgHeight / (float) source.getHeight();
            } else if (mImgWidth != Constants.UNDEFINED) {
                sx = sy = mImgWidth / (float) source.getWidth();
            } else if (mImgHeight != Constants.UNDEFINED) {
                sy = sx = mImgHeight / (float) source.getHeight();
            }

            if (sx != 1 || sy != 1) {
                sScaleMatrix.setScale(sx, sy);
                result = Bitmap.createBitmap(source, 0, 0, source.getWidth(),
                        source.getHeight(), sScaleMatrix, true);
                source.recycle();
            } else {
                result = source;
            }


            return result;
        }

        @Override
        public String key() {
            return mKey;
        }

    }
}
