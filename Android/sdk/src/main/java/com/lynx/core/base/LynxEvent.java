// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.core.base;

public class LynxEvent extends LynxObject {

    protected static final String TYPE = "type";

    public LynxEvent(String type) {
        setProperty(TYPE, type);
    }
}
