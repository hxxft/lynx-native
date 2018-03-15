// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.test;

import android.app.Activity;
import android.os.Bundle;
import android.support.annotation.Nullable;

import com.lynx.ui.LynxView;
import com.lynx.utils.StringUtil;

import java.util.Map;

public class TestActivity extends Activity {
    protected LynxView mLynxView;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        mLynxView = new LynxView(this);
        setContentView(mLynxView);
    }

    public void loadBundle(String assetName, Map<String, Object> objectsExposedToJS) {
        if (objectsExposedToJS != null) {
            for (String name : objectsExposedToJS.keySet()) {
                mLynxView.addJavascriptInterface(objectsExposedToJS.get(name), name);
            }
        }
        mLynxView.loadScriptData(readBundleScript(assetName + ".js"));
    }

    private String readBundleScript(String fileName) {
        try {
            return StringUtil.convertToString(getResources().getAssets().open(fileName));
        } catch (Exception e) {
            e.printStackTrace();
        }
        return null;
    }

    public LynxView getLynxView() {
        return mLynxView;
    }
}
