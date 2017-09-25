// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.ui.drawable;

import android.graphics.Canvas;
import android.support.annotation.NonNull;

import com.lynx.base.Position;
import com.lynx.base.Style;

public interface IMaker {
    void updateBounds(Position bounds);

    void updateStyle(Style style);

    void draw(@NonNull Canvas canvas);
}
