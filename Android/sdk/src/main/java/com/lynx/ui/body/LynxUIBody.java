// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.ui.body;

import android.content.Context;

import com.lynx.base.SupposeUIThread;
import com.lynx.core.base.LynxObject;
import com.lynx.core.tree.LynxUIAction;
import com.lynx.ui.LynxView;
import com.lynx.ui.coordinator.CrdResponder;
import com.lynx.ui.coordinator.CrdSponsor;
import com.lynx.ui.coordinator.CrdTransferStation;
import com.lynx.ui.coordinator.TransferStation;
import com.lynx.ui.view.AndroidViewGroup;
import com.lynx.ui.view.LynxUIView;

public class LynxUIBody extends LynxUIView implements TransferStation {
    private CrdTransferStation mCrdTransferStation;

    public LynxUIBody(Context context) {
        super(context);
        mCrdTransferStation = new CrdTransferStation();
    }

    @Override
    protected AndroidViewGroup createView(Context context) {
        // No-op as creating view is handled by {@Link LynxView}
        return null;
    }

    public void bindView(AndroidBody view) {
        mView = view;
    }

    @Override
    public void measure() {
        for (int i = 0; i < mRenderObjectImpl.getChildCount(); ++i) {
            if (mRenderObjectImpl.getChildAt(i).hasUI()) {
                mRenderObjectImpl.getChildAt(i).getUI().measure();
            }
        }
    }

    @Override
    public void layout() {
        for (int i = 0; i < mRenderObjectImpl.getChildCount(); ++i) {
            if (mRenderObjectImpl.getChildAt(i).hasUI()) {
                mRenderObjectImpl.getChildAt(i).getUI().layout();
            }
        }
    }

    @SupposeUIThread
    public void collect(LynxUIAction action) {
        ((LynxView) mView).getHostImpl().collect(action);
    }

    @Override
    public void updatePropertiesInAction(String sponsorAffinity, String responderAffinity,
                                         LynxObject object, boolean notify) {
        mCrdTransferStation.updatePropertiesInAction(sponsorAffinity, responderAffinity, object, notify);
    }

    @Override
    public void addExecutableAction(String sponsorAffinity, String responderAffinity,
                                    String executable) {
        mCrdTransferStation.addExecutableAction(sponsorAffinity, responderAffinity, executable);
    }

    @Override
    public void removeExecutableAction(String sponsorAffinity, String responderAffinity) {
        mCrdTransferStation.removeExecutableAction(sponsorAffinity, responderAffinity);
    }

    @Override
    public void removeAllExecutableAction() {
        mCrdTransferStation.removeAllExecutableAction();
    }

    @Override
    public void addCoordinatorResponder(CrdResponder responder) {
        mCrdTransferStation.addCoordinatorResponder(responder);
    }

    @Override
    public void removeCoordinatorResponder(CrdResponder responder) {
        mCrdTransferStation.removeCoordinatorResponder(responder);
    }

    @Override
    public void addCoordinatorSponsor(CrdSponsor sponsor) {
        mCrdTransferStation.addCoordinatorSponsor(sponsor);
    }

    @Override
    public void removeCoordinatorSponsor(CrdSponsor sponsor) {
        mCrdTransferStation.removeCoordinatorSponsor(sponsor);
    }

    @Override
    public boolean dispatchNestedAction(String type, CrdSponsor sponsor, Object... params) {
        return mCrdTransferStation.dispatchNestedAction(type, sponsor, params);
    }

}
