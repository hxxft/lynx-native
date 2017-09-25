// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.ui.anim;

public enum AnimTimingFunction {
    LINEAR("linear"),
    EASE("ease"),
    EASE_IN("ease-in"),
    EASE_OUT("ease-out"),
    EASE_IN_OUT("ease-in-out");

    private String mName;

    AnimTimingFunction(String name) {
        mName = name;
    }

    public static AnimTimingFunction is(String name) {
        if (name.equals("linear")) {
            return LINEAR;
        } else if (name.equals("ease")) {
            return EASE;
        } else if (name.equals("ease-in")) {
            return EASE_IN;
        } else if (name.equals("ease-out")) {
            return EASE_OUT;
        } else if (name.equals("ease-in-out")) {
            return EASE_IN_OUT;
        }
        return LINEAR;
    }
}
