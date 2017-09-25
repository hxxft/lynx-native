// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.core.tree;

import android.util.SparseArray;

import com.lynx.core.impl.RenderObjectImpl;

import java.util.HashMap;
import java.util.LinkedHashMap;
import java.util.Map;

public class LynxUIActionCollector {

    private Map<RenderObjectImpl, Map<String, LynxUIAction>> mEventActions;
    private Map<RenderObjectImpl, SparseArray<LynxUIAction>> mUpdateDataActions;

    public LynxUIActionCollector() {
        mEventActions = new LinkedHashMap<>();
        mUpdateDataActions = new LinkedHashMap<>();
    }

    public void collect(LynxUIAction action) {
        switch (action.type()) {
            case DO_EVENT_ACTION: {
                Map<String, LynxUIAction> actions = mEventActions.get(action.target());
                if (actions == null) {
                    actions = new HashMap<>();
                    mEventActions.put(action.target(), actions);
                }
                actions.put(((LynxUIEventAction) action).event(), action);
            }
                break;
            case DO_UPDATE_DATA_ACTION: {
                SparseArray<LynxUIAction> actions = mUpdateDataActions.get(action.target());
                if (actions == null) {
                    actions = new SparseArray<>();
                    mUpdateDataActions.put(action.target(), actions);
                }
                actions.put(((LynxUIUpdateDataAction) action).key().value(), action);
            }
                break;
            case DO_EVENT_NONE:
            default:
                break;
        }
    }

    public boolean needDoActions() {
        return !(mEventActions.isEmpty() && mUpdateDataActions.isEmpty());
    }

    public void doActions() {
        for (Map.Entry<RenderObjectImpl, SparseArray<LynxUIAction>> entry
                : mUpdateDataActions.entrySet()) {

            SparseArray<LynxUIAction> actions = entry.getValue();
            for (int i = 0; i < actions.size(); ++i) {
                LynxUIAction action = actions.valueAt(i);
                action.doAction();
            }
        }
        for (Map.Entry<RenderObjectImpl, Map<String, LynxUIAction>> entry1
                : mEventActions.entrySet()) {

            Map<String, LynxUIAction> actions = entry1.getValue();

            for (Map.Entry<String, LynxUIAction> entry2 : actions.entrySet()) {
                LynxUIAction action = entry2.getValue();
                action.doAction();
            }
        }
    }

}
