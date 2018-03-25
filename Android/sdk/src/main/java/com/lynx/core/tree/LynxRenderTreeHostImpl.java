// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.core.tree;

import com.lynx.base.CalledByNative;
import com.lynx.base.FrameRateController;
import com.lynx.base.Position;
import com.lynx.base.SupposeUIThread;
import com.lynx.core.impl.RenderObjectImpl;
import com.lynx.ui.LynxUI;

public class LynxRenderTreeHostImpl implements FrameRateController.VSyncListener {

    private native void nativeUpdateViewport(long hostImpl, int left, int top, int right, int bottom);
    private native void nativeOnVSync(long hostImpl);
    private long mHostImpl;

    public RenderObjectImpl mRootRender;
    private LynxUIActionCollector mCollector = new LynxUIActionCollector();
    private LynxUIActionCollector mLastCollector = null;

    public LynxRenderTreeHostImpl(long hostImpl, RenderObjectImpl root) {
        mHostImpl = hostImpl;
        mRootRender = root;
    }

    @Override
    public void OnVSync() {
        nativeOnVSync(mHostImpl);
    }

    // Called oon UI thread
    @CalledByNative
    public void beignFrame() {
        mLastCollector = mCollector;
        mCollector = new LynxUIActionCollector();
    }

    // Called on JS thread
    @CalledByNative
    public void prepareCommit() {
        if(mLastCollector != null && mLastCollector.needDoActions()) {
            mLastCollector.doActions();
        }
    }

    public void freeNativeJavaRefs(){
        mRootRender.freeJavaRef();
    }

    public void constructJavaRef(){
        mRootRender.constructJavaRef();
    }

    public void setRootUI(LynxUI root) {
        mRootRender.setUI(root);
    }

    public void updateViewport(Position viewport) {
        nativeUpdateViewport(mHostImpl, viewport.mLeft, viewport.mTop,
                viewport.mRight, viewport.mBottom);
    }

    // Should be called on UI thread
    @SupposeUIThread
    public void collect(LynxUIAction action) {
        mCollector.collect(action);
    }

    @CalledByNative
    static LynxRenderTreeHostImpl create(long hostImpl, Object root) {
        return new LynxRenderTreeHostImpl(hostImpl, (RenderObjectImpl)(root));
    }
}
