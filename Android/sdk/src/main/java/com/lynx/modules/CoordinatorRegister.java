// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.modules;

import com.lynx.core.LynxMethod;
import com.lynx.core.LynxRuntime;
import com.lynx.core.base.LynxObject;
import com.lynx.ui.LynxUI;
import com.lynx.ui.coordinator.TransferStation;

public class CoordinatorRegister extends LynxModule {
    private LynxRuntime mRuntime;

    public CoordinatorRegister(LynxRuntime runtime) {
        mRuntime = runtime;
    }

    @LynxMethod
    public void registerAction(final String sponsorAffinity, final String responderAffinity,
                              final String executable) {
        final LynxUI ui = mRuntime.getHost().mRootRender.getUI();
        ui.getView().post(new Runnable() {
            @Override
            public void run() {
                ((TransferStation) ui)
                        .addExecutableAction(sponsorAffinity, responderAffinity, executable);
            }
        });
    }

    @LynxMethod
    public void removeAction(final String sponsorAffinity, final String responderAffinity) {
        final LynxUI ui = mRuntime.getHost().mRootRender.getUI();
        ui.getView().post(new Runnable() {
            @Override
            public void run() {
                ((TransferStation) ui).removeExecutableAction(sponsorAffinity, responderAffinity);
            }
        });
    }

    @LynxMethod
    public void updateProperties(final String sponsorAffinity, final String responderAffinity,
                                 final LynxObject object, final boolean notify) {
        final LynxUI ui = mRuntime.getHost().mRootRender.getUI();
        ui.getView().post(new Runnable() {
            @Override
            public void run() {
                ((TransferStation) ui).updatePropertiesInAction(sponsorAffinity, responderAffinity,
                        object, notify);
            }
        });
    }

    @Override
    public String moduleName() {
        return "LynxCoordinatorRegister";
    }

    @Override
    public void onDestroy() {
        ((TransferStation) mRuntime.getHost().mRootRender.getUI())
                .removeAllExecutableAction();
    }
}
