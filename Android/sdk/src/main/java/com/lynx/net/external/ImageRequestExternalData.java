// Copyright 2017 The Lynx Authors. All rights reserved.

package com.lynx.net.external;

import android.widget.ImageView;


public class ImageRequestExternalData {
    private ImageView mImageView;
    private ImageTransformation mImageTransformation;

    public ImageRequestExternalData(ImageView imageView, ImageTransformation transformation) {
        mImageView = imageView;
        mImageTransformation = transformation;
    }

    public ImageRequestExternalData(ImageTransformation transformation) {
        mImageTransformation = transformation;
    }

    public ImageView getImageView() {
        return mImageView;
    }

    public ImageTransformation getTransformation() {
        return mImageTransformation;
    }
}
