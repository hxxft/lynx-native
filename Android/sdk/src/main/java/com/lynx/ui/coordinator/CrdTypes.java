// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.ui.coordinator;

import android.text.TextUtils;

import java.util.HashSet;
import java.util.Set;

public class CrdTypes {
    private final static String TYPE_DELIMITER = "\\|";

    public final static String SCROLL = "scroll";
    public final static String TOUCH = "touch";

    public Set<String> mTypes;
    private String mRawContent;

    public CrdTypes(String content) {
        mRawContent = content;
        mTypes = new HashSet<>();
        if (!TextUtils.isEmpty(content)) {
            String[] types = content.split(TYPE_DELIMITER);
            for (String type : types) {
                mTypes.add(type.trim());
            }
        }
    }

    public boolean hasType(String type) {
        return mTypes.contains(type);
    }

    public String getRawContent() {
        return mRawContent;
    }
}
