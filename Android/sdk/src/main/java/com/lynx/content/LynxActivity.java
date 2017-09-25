// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.content;

import android.app.Activity;
import android.os.Bundle;
import android.support.annotation.Nullable;

public class LynxActivity extends Activity {
    private LynxActivityDelegate mDelegate;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        mDelegate = new LynxActivityDelegate(this);
        mDelegate.onCreate(savedInstanceState);
    }

    @Override
    protected void onStart() {
        super.onStart();
        mDelegate.onStart();
    }

    @Override
    protected void onStop() {
        super.onStop();
        mDelegate.onStop();
    }

}
