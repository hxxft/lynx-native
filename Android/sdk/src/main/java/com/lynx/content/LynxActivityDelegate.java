// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.content;

import android.app.Activity;
import android.content.Intent;
import android.graphics.Color;
import android.os.Bundle;
import android.support.annotation.Nullable;

import com.lynx.content.info.App;
import com.lynx.content.info.Page;
import com.lynx.dev.DebugDevHost;
import com.lynx.dev.DevSupportManager;
import com.lynx.resources.ResourceManager;
import com.lynx.ui.LynxView;
import com.lynx.utils.ScreenUtil;

public class LynxActivityDelegate implements DebugDevHost{
    private Activity mActivity;
    private LynxView mLynxView;

    public LynxActivityDelegate(Activity activity) {
        mActivity = activity;
    }

    public void onCreate(@Nullable Bundle savedInstanceState) {

        DevSupportManager.getInstance().setDebugDevHost(this);

        mLynxView = new LynxView(mActivity);

        Intent intent = mActivity.getIntent();
        String pageName = intent.getStringExtra("page");

        App app = ((LynxApplication) mActivity.getApplication()).getAppInfo();
        if (Intent.ACTION_MAIN.equals(intent.getAction())
                && intent.getCategories().contains(Intent.CATEGORY_LAUNCHER)) {
            pageName = app.getMainPage();
        }

        if (!app.isLegalPage(pageName)) {
            mActivity.finish();
            return;
        }

        Page currentPage = app.getPage(pageName);

        mLynxView.loadHTMLData(currentPage.getResourceURL(),
                ResourceManager.instance().reader().readResourceAsString(
                        ResourceManager.toRealURL(currentPage.getLayoutResourceURL())));

        mLynxView.setBackgroundColor(Color.WHITE);
        mActivity.setContentView(mLynxView);
    }

    @Override
    public void debugHotReload(String url) {
        LynxView view = mLynxView;
        mLynxView = new LynxView(mActivity);
        mLynxView.loadUrl(url);
        mLynxView.setBackgroundColor(Color.WHITE);
        mActivity.setContentView(mLynxView);
        view.destroy();
    }

    public void onStart() {
        mLynxView.onStart();
    }

    public void onStop() {
        mLynxView.onStop();
    }

}
