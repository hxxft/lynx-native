// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.ui.image;

import android.graphics.Bitmap;
import android.graphics.BitmapShader;
import android.graphics.Canvas;
import android.graphics.Matrix;
import android.graphics.Paint;
import android.graphics.RectF;

import com.lynx.base.Style;
import com.lynx.core.impl.RenderObjectImpl;
import com.lynx.net.external.ImageTransformation;
import com.lynx.ui.utils.RoundRectHelper;

public class LynxUIImageTransformation implements ImageTransformation {
    private String mKey;
    private int mHeight;
    private int mWidth;
    private float mBorderRadius;
    private float mBorderWidth;
    private int mObjectFitType;

    public LynxUIImageTransformation(RenderObjectImpl renderObjectImpl) {
        if (renderObjectImpl == null) {
            mKey = "null";
        } else {
            mHeight = renderObjectImpl.getPosition().getHeight();
            mWidth = renderObjectImpl.getPosition().getWidth();
            mBorderRadius = (float) renderObjectImpl.getStyle().mBorderRadius;
            mBorderWidth = (float) renderObjectImpl.getStyle().mBorderWidth;
            mObjectFitType = renderObjectImpl.getStyle().mObjectFit;
            // Set unique key
            StringBuilder builder = new StringBuilder();
            // height
            builder.append(mHeight).append(";");
            // width
            builder.append(mWidth).append(";");
            // border radius
            builder.append(mBorderRadius).append(";");
            // object-fit
            builder.append(mObjectFitType).append(";");
            // border width
            builder.append(mBorderWidth).append(";");
            mKey = builder.toString();
        }
    }

    /* Processing image with object-fit and radius style ahead */
    public Bitmap transform(Bitmap source) {
        // Do noting while no radius style or height 0 or width 0
        if (mWidth == 0 || mHeight == 0 || mBorderRadius == 0) {
            return source;
        }
        int w = mWidth;
        int h = mHeight;
        // Create other bitmap without compression
        Bitmap bitmap = Bitmap.createBitmap(w, h, source.getConfig());
        RectF rectF = null;
        Matrix matrix = new Matrix();
        Canvas canvas = new Canvas(bitmap);
        canvas.drawARGB(0, 0, 0, 0);
        Paint paint = new Paint(Paint.ANTI_ALIAS_FLAG);
        // Using bitmapShader in paint to draw image.
        BitmapShader shader = new BitmapShader(source, BitmapShader.TileMode.CLAMP,
                BitmapShader.TileMode.CLAMP);
        // source width and height
        float sourceWidth = source.getWidth();
        float sourceHeight = source.getHeight();
        float borderWidth = mBorderWidth;
        float availableWidth = w - 2 * borderWidth;
        float availableHeight = h - 2 * borderWidth;
        float offsetStartX = borderWidth;
        float offsetStartY = borderWidth;
        // Scale image
        switch (mObjectFitType){
            case Style.CSSIMAGE_OBJECT_FIT_FILL:
                // Scale to fix the width and height
                matrix.setScale(availableWidth / sourceWidth, availableHeight / sourceHeight);
                matrix.postTranslate(offsetStartX, offsetStartY);
                rectF = new RectF(
                        offsetStartX,
                        offsetStartY,
                        offsetStartX + availableWidth,
                        offsetStartY + availableHeight);
                break;
            case Style.CSSIMAGE_OBJECT_FIT_CONTAIN:
                // Accroding to the width and height factor to shrink the image displaying center
            {
                float w_rate = availableWidth / sourceWidth;
                float h_rate = availableHeight / sourceHeight;
                if (w_rate > h_rate) {
                    float finalWidth = sourceWidth * h_rate;
                    float startW = (availableWidth - finalWidth) / 2 + offsetStartX;
                    float startH = offsetStartY;
                    rectF = new RectF(
                            startW,
                            startH,
                            startW + finalWidth,
                            startH + availableHeight);
                    matrix.setScale(h_rate, h_rate);
                    matrix.postTranslate(startW, startH);
                } else {
                    float finalHeight = sourceHeight * w_rate;
                    float startW = offsetStartX;
                    float startH = (availableHeight - finalHeight) / 2 + offsetStartX;
                    rectF = new RectF(
                            startW,
                            startH,
                            startW + availableWidth,
                            startH + finalHeight);
                    matrix.setScale(w_rate, w_rate);
                    matrix.postTranslate(startW, startH);
                }
            }
            break;
            case Style.CSSIMAGE_OBJECT_FIT_COVER:
                // Accroding to the width and height factor to enlarge the image displaying center
            {
                float w_rate = availableWidth / sourceWidth;
                float h_rate = availableHeight / sourceHeight;
                if (w_rate > h_rate) {
                    float finalHeight = sourceHeight * w_rate;
                    float translateX = offsetStartX;
                    float translateY = (availableHeight - finalHeight) / 2 + offsetStartY;
                    rectF = new RectF(
                            offsetStartX,
                            offsetStartY,
                            offsetStartX + availableWidth,
                            offsetStartY + availableHeight);
                    matrix.setScale(w_rate, w_rate);
                    matrix.postTranslate(translateX, translateY);
                } else {
                    float finalWidth = sourceWidth * h_rate;
                    float translateX = (availableWidth - finalWidth) / 2 + offsetStartX;
                    float translateY = offsetStartY;
                    rectF = new RectF(
                            offsetStartX,
                            offsetStartY,
                            offsetStartX + availableWidth,
                            offsetStartY + availableHeight);
                    matrix.setScale(h_rate, h_rate);
                    matrix.postTranslate(translateX, translateY);
                }
            }
            break;
            default:
                break;
        }
        shader.setLocalMatrix(matrix);
        paint.setShader(shader);
        canvas.drawRect(rectF, paint);
        // If radius > 0, processing corner
        float radius = mBorderRadius;
        if (radius > 0) {
            paint.setShader(null);
            RoundRectHelper.instance()
                    .clipRoundRectWithXfermode(canvas,
                            mWidth,
                            mHeight,
                            radius,
                            radius,
                            source.getConfig());
        }
        // Recycle origin bitmap
        source.recycle();
        return bitmap;
    }

    public String key() {
        return mKey;
    }

}
