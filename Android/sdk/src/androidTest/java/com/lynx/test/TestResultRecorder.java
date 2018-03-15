// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.test;

import android.webkit.JavascriptInterface;

import java.util.LinkedHashMap;

public class TestResultRecorder {
    public static final String TAG = "TestResultRecorder";

    private final LinkedHashMap<String, String> mResults = new LinkedHashMap<>();

    @JavascriptInterface
    public void record(String key, String value) {
        mResults.put(key, value);
    }

    public void reset() {
        mResults.clear();
    }

    public LinkedHashMap<String, String> getResults() {
        return mResults;
    }
}
