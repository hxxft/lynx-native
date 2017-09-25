// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.ui.image;

import android.content.Context;
import android.graphics.Canvas;
import android.widget.ImageView;

import com.lynx.core.impl.RenderObjectImpl;
import com.lynx.ui.utils.RoundRectHelper;

/**
 * Created by dli on 6/21/16.
 */
public class AndroidImage extends ImageView {

    private LynxUIImage mUI;
    private boolean isAnimatedImage = false;

    public AndroidImage(Context context, LynxUIImage presenter) {
        super(context);
        mUI = presenter;
    }

    @Override
    public void draw(Canvas canvas) {
        RenderObjectImpl impl = mUI.getRenderObjectImpl();
        if (isAnimatedImage
                && impl != null
                && impl.getStyle().mBorderRadius > 0) {
            // Clipping canvas for radius
            int count = canvas.saveLayer(
                    0,
                    0,
                    impl.getPosition().getWidth(),
                    impl.getPosition().getHeight(),
                    null,
                    Canvas.HAS_ALPHA_LAYER_SAVE_FLAG);
            super.draw(canvas);
            RoundRectHelper.instance().clipRoundRectWithXfermode(canvas, impl);
            canvas.restoreToCount(count);
        } else {
            // Do not clip
            super.draw(canvas);
        }
    }

    @Override
    protected void onDraw(Canvas canvas) {
        if (getDrawable() == null) {
            super.onDraw(canvas);
        }

        RenderObjectImpl impl = mUI.getRenderObjectImpl();
        // When style has border attribute, clipping canvas to keep border width.
        // This action should active when style hasn't radius attribute,
        // otherwise do it in ImageTransformation.
        if (impl != null
                && impl.getStyle().mBorderWidth > 0
                && impl.getStyle().mBorderRadius == 0) {
            int borderWidth = (int) impl.getStyle().mBorderWidth;
            int count = canvas.save();
            canvas.clipRect(
                    borderWidth,
                    borderWidth,
                    impl.getPosition().getWidth() - borderWidth,
                    impl.getPosition().getHeight() - borderWidth);
            // Draw the image
            super.onDraw(canvas);
        canvas.restoreToCount(count);
        } else {
            super.onDraw(canvas);
        }
    }


    @Override
    protected void onAttachedToWindow() {
        super.onAttachedToWindow();
        mUI.onAttachedToWindow();

    }

    @Override
    protected void onDetachedFromWindow() {
        super.onDetachedFromWindow();
        mUI.onDetachedFromWindow();
    }
}
