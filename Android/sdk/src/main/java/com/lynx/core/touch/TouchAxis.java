// Copyright 2017 The Lynx Authors. All rights reserved.

package com.lynx.core.touch;

/* package */  class TouchAxis {
    public float x;
    public float y;

    public TouchAxis() {

    }

    public TouchAxis(TouchAxis axis) {
        this.x = axis.x;
        this.y = axis.y;
    }

    public void setXY(float x, float y) {
        this.x = x;
        this.y = y;
    }

    public void setX(float x) {
        this.x = x;
    }

    public void setY(float y) {
        this.y = y;
    }
}
