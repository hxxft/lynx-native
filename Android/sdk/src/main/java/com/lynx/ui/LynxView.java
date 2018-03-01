// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.ui;

import android.content.Context;
import android.view.View;

import com.lynx.base.FrameRateController;
import com.lynx.base.Size;
import com.lynx.core.LynxRuntime;
import com.lynx.core.LynxRuntimeManager;
import com.lynx.core.ResultCallback;
import com.lynx.core.tree.LynxRenderTreeHostImpl;
import com.lynx.ui.body.AndroidBody;
import com.lynx.utils.StringUtil;

import java.io.FileInputStream;
import java.io.FileNotFoundException;

public class LynxView extends AndroidBody {

    private Size mMeasuredSize = new Size(0, 0);

    protected LynxRuntime mRuntime;

    private boolean mNeedStartCtrl = false;

    private FrameRateController mController;
    private LynxRenderTreeHostImpl mRenderTreeHostImpl;


    public LynxView(Context context) {
        super(context);

        mRuntime = LynxRuntimeManager.getIdleRuntime();
        mRenderTreeHostImpl = mRuntime.active(this);

        mRenderTreeHostImpl.setRootUI(mUIGroup);
        mUIGroup.bindData(mRenderTreeHostImpl.mRootRender);

        mController = new FrameRateController(mRenderTreeHostImpl);
    }

    public void loadUrl(String url) {
        mRuntime.loadUrl(url);
    }

    public void loadScriptData(String data) {
        mRuntime.runScript(data);
    }

    public void loadScriptData(String data, ResultCallback callback) {
        mRuntime.runScript(data, callback);
    }

    public void loadScriptDataWithBaseUrl(String data, String baseUrl) {
        mRuntime.loadScriptDataWithBaseUrl(data, baseUrl);
    }

    public void loadHTMLData(String url, String data) {
        mRuntime.loadHTML(url, data);
    }

    public void loadHTMLDataWithBaseUrl(String url, String data) {
        mRuntime.loadHTML(url, data);
    }

    public void loadScriptFile(String file) throws FileNotFoundException {
        String content = StringUtil.convertToString(new FileInputStream(file));
        mRuntime.runScript(content);
    }

    public void addJavascriptInterface(Object object, String name) {
        mRuntime.addJavascriptInterface(object, name);
    }

    @Override
    protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
        super.onMeasure(widthMeasureSpec, heightMeasureSpec);
        int width = View.MeasureSpec.getSize(widthMeasureSpec);
        int height = View.MeasureSpec.getSize(heightMeasureSpec);
        if(mMeasuredSize.update(width, height) && mRuntime != null) {
            mRenderTreeHostImpl.updateViewport(width, height);
        }
    }

    @Override
    protected void onAttachedToWindow() {
        super.onAttachedToWindow();
        mController.start();
        mNeedStartCtrl = true;
    }

    public void onStop(){
        mController.stop();
        mRenderTreeHostImpl.freeNativeJavaRefs();
    }

    public void onStart(){
        mRenderTreeHostImpl.constructJavaRef();
        if (mNeedStartCtrl) {
            mController.start();
        }
    }

    @Override
    protected void onDetachedFromWindow() {
        super.onDetachedFromWindow();
        mRuntime.destroy();
    }

    public void destroy() {
        mController.stop();
        mRuntime.destroy();
    }

    public LynxRenderTreeHostImpl getHostImpl() {
        return mRenderTreeHostImpl;
    }
}
