// Copyright 2017 The Lynx Authors. All rights reserved.

package com.lynx.base;

public class Size {
    public double mWidth;
    public double mHeight;

    public Size() {
        this.mWidth = 0;
        this.mHeight = 0;
    }

    public Size(double width, double height) {
        this.mWidth = width;
        this.mHeight = height;
    }

    public boolean isEqual(Size size) {
        return mHeight == size.mHeight &&
                mWidth == size.mWidth;
    }

    public boolean update(int width, int height) {
        if(mWidth == width && mHeight == height) {
            return false;
        }
        this.mWidth = width;
        this.mHeight = height;
        return true;
    }
}
