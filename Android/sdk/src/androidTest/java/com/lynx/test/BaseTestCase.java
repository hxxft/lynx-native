// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.test;

import android.test.ActivityInstrumentationTestCase2;

import java.util.Map;

public abstract class BaseTestCase
        extends ActivityInstrumentationTestCase2<TestActivity> {

    public BaseTestCase() {
        super(TestActivity.class);
    }

    @Override
    protected void setUp() throws Exception {
        super.setUp();
        try {
            final TestActivity activity = getActivity();
            runTestOnUiThread(new Runnable() {
                @Override
                public void run() {
                    activity.loadBundle(getBundleNameForTest(),
                            getExposedToJSObjectList());
                }
            });
        } catch (Throwable throwable) {
            throw new Exception("Unable to load bundle", throwable);
        }
    }

    public abstract String getBundleNameForTest();

    public Map<String, Object> getExposedToJSObjectList() {
        return null;
    }

}
