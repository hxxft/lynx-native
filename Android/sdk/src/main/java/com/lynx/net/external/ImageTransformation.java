// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.net.external;

import android.graphics.Bitmap;

public interface ImageTransformation {

    Bitmap transform(Bitmap source);

    String key();

}
