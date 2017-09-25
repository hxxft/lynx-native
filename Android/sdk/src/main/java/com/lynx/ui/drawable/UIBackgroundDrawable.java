// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.ui.drawable;

import android.graphics.Canvas;
import android.graphics.ColorFilter;
import android.graphics.PixelFormat;
import android.graphics.drawable.Drawable;
import android.support.annotation.IntRange;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;

import com.lynx.base.Position;
import com.lynx.base.Style;

import java.util.ArrayList;
import java.util.List;

public class UIBackgroundDrawable extends Drawable {
    protected List<IMaker> mMakers;

    public UIBackgroundDrawable() {
        mMakers = new ArrayList<>();
        // This adding order will affect the drawing order
        mMakers.add(new BackgroundColorMaker());
        mMakers.add(new BorderMaker());
    }

    public void updateBounds(@NonNull Position position) {
        for (IMaker maker : mMakers) {
            maker.updateBounds(position);
        }
    }

    public void updateStyle(@NonNull Style style) {
        for (IMaker maker : mMakers) {
            maker.updateStyle(style);
        }
    }

    @Override
    public void draw(@NonNull Canvas canvas) {
        for (IMaker maker : mMakers) {
            maker.draw(canvas);
        }
    }

    @Override
    public void setAlpha(@IntRange(from = 0, to = 255) int alpha) {

    }

    @Override
    public void setColorFilter(@Nullable ColorFilter colorFilter) {

    }

    @Override
    public int getOpacity() {
        return PixelFormat.OPAQUE;
    }
}
