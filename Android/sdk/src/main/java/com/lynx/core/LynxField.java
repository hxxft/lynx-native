// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.core;

import java.lang.annotation.ElementType;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;
import java.lang.annotation.Target;

@SuppressWarnings("javadoc")
@Retention(RetentionPolicy.RUNTIME)
@Target({ElementType.METHOD})
public @interface LynxField {

    String field();

    Method method();

    enum Method {
        Get, Set
    }
}
