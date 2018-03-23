// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.core.annotation;

import java.lang.annotation.ElementType;
import java.lang.annotation.Target;


@Target({ElementType.TYPE})
public @interface JSObject {
    String name();
}
