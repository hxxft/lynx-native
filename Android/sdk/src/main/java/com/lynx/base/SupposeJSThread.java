// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.base;

import java.lang.annotation.ElementType;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;
import java.lang.annotation.Target;

/**
 * Annotate your methods to ensure they are called
 * from the JS thread
 */
@Target({ElementType.METHOD})
@Retention(RetentionPolicy.CLASS)
public @interface SupposeJSThread {
}
