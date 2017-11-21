// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.ui.body;

import android.content.Context;
import android.view.MotionEvent;

import com.lynx.base.SupposeUIThread;
import com.lynx.core.tree.LynxUIAction;
import com.lynx.ui.LynxView;
import com.lynx.ui.coordinator.CommandExecutor;
import com.lynx.ui.coordinator.CoordinatorResponder;
import com.lynx.ui.coordinator.CoordinatorSponsor;
import com.lynx.ui.coordinator.CoordinatorTypes;
import com.lynx.ui.coordinator.PreTreatment;
import com.lynx.ui.coordinator.TransferStation;
import com.lynx.ui.view.AndroidViewGroup;
import com.lynx.ui.view.LynxUIView;

import java.util.HashMap;
import java.util.HashSet;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.Set;

public class LynxUIBody extends LynxUIView implements TransferStation {

    private Map<String, Set<CoordinatorResponder>> mCoordinatorResponders;
    private Map<String, CommandExecutor> mExecutorPool;
    private Map<String, Set<String>> mAffinityRelationShip;
    private PreTreatment mPreTreatment;

    public LynxUIBody(Context context) {
        super(context);
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
    public void addExecutableAction(String sponsorAffinity, String responderAffinity, String executable) {
        if (mAffinityRelationShip == null) {
            mAffinityRelationShip = new HashMap<>();
        }
        if (mExecutorPool == null) {
            mExecutorPool = new HashMap<>();
        }
        Set<String> responderAffinityList = mAffinityRelationShip.get(sponsorAffinity);
        if (responderAffinityList == null) {
            responderAffinityList = new HashSet<>();
            mAffinityRelationShip.put(sponsorAffinity, responderAffinityList);
        }
        responderAffinityList.add(responderAffinity);

        CommandExecutor executor = mExecutorPool.get(responderAffinity);
        if (executor == null) {
            executor = new CommandExecutor(executable);
            mExecutorPool.put(responderAffinity, executor);
        } else {
            executor.update(executable);
        }

        if (mPreTreatment == null) {
            mPreTreatment = new PreTreatment();
        }
        // Init
        if (mCoordinatorResponders != null) {
            Set<CoordinatorResponder> set = mCoordinatorResponders.get(responderAffinity);
            if (set != null) {
                for (CoordinatorResponder responder : set) {
                    responder.coordinatorTreatment().init(executor,
                            responder.coordinatorTag() == null ? 0 : Integer.valueOf(responder.coordinatorTag()));
                }
            }
        }
    }

    @Override
    public void removeExecutableAction(String sponsorAffinity, String responderAffinity) {
        if (mAffinityRelationShip == null || mExecutorPool == null) {
            return;
        }
        Set<String> responderAffinityList = mAffinityRelationShip.get(sponsorAffinity);
        if (responderAffinityList == null) {
            return;
        }
        mAffinityRelationShip.remove(responderAffinityList);
        mExecutorPool.remove(responderAffinity);
    }

    @Override
    public void removeAllExecutableAction() {
        mAffinityRelationShip.clear();
        mExecutorPool.clear();
    }

    @Override
    public void addCoordinatorResponder(CoordinatorResponder responder) {
        if (mCoordinatorResponders == null) {
            mCoordinatorResponders = new HashMap<>();
        }
        Set<CoordinatorResponder> set = mCoordinatorResponders.get(responder.coordinatorAffinity());
        if (set == null) {
            set = new HashSet<>();
            mCoordinatorResponders.put(responder.coordinatorAffinity(), set);
        }
        set.add(responder);
        // Init
        if (mExecutorPool != null) {
            CommandExecutor executor = mExecutorPool.get(responder.coordinatorAffinity());
            if (executor != null) {
                responder.coordinatorTreatment().init(executor,
                        responder.coordinatorTag() == null ? 0 : Integer.valueOf(responder.coordinatorTag()));
            }
        }
    }

    @Override
    public void removeCoordinatorResponder(CoordinatorResponder responder) {
        if (mCoordinatorResponders != null && responder != null) {
            Set<CoordinatorResponder> set = mCoordinatorResponders.get(responder.coordinatorAffinity());
            if (set != null) {
                set.remove(responder);
            }
        }
    }

    @Override
    public boolean dispatchNestedAction(String type, CoordinatorSponsor sponsor, Object... params) {
        boolean consumed = false;
        if (mCoordinatorResponders != null
                && mExecutorPool != null
                && mAffinityRelationShip != null) {
            Set<String> responderAffinityList =
                    mAffinityRelationShip.get(sponsor.coordinatorAffinity());
            if (responderAffinityList == null) return consumed;
            for (String responderAffinity : responderAffinityList) {
                CommandExecutor executor = mExecutorPool.get(responderAffinity);
                Set<CoordinatorResponder> set = mCoordinatorResponders.get(responderAffinity);
                if (executor == null || set == null) continue;
                // TODO: 17/11/7 tag should be replace to string
                consumed = mPreTreatment.dispatchAction(type, executor,
                        sponsor.coordinatorTag(), params);
                for (CoordinatorResponder responder : set) {
                    responder.coordinatorTreatment().onNestedAction(type, executor, params);
                }
            }
        }
        return consumed;
    }

}
