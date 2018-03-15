// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.core.tree;

import com.lynx.core.impl.RenderObjectImpl;

import java.util.HashMap;
import java.util.LinkedList;
import java.util.Map;

/* package */  class LynxUIActionCollector {

    private Map<RenderObjectImpl, Map<String, LynxUIAction.UnorderedAction>> mActionsMap;
    private LinkedList<LynxUIAction.OrderedAction> mActionsList;

    /* package */  LynxUIActionCollector() {
        mActionsMap = new HashMap<>();
        mActionsList = new LinkedList<>();
    }

    public void collect(LynxUIAction action) {
        switch (action.type()) {
            case DO_ORDERED_ACTION:
                collectOrderedAction((LynxUIAction.OrderedAction) action);
                break;
            case DO_UNORDERED_ACTION:
                collectUnorderedAction((LynxUIAction.UnorderedAction) action);
                break;
            case DO_EVENT_NONE:
            default:
                break;
        }
    }

    private void collectOrderedAction(LynxUIAction.OrderedAction action) {
        if (!action.canCoalesce() || mActionsList.isEmpty()) {
            mActionsList.add(action);
            return;
        }
        LynxUIAction.OrderedAction last = mActionsList.getLast();
        if (action.target() == last.target()
                && last.getCoalescingKey() == action.getCoalescingKey()) {
            mActionsList.removeLast();
            mActionsList.add(action);
        }
    }

    private void collectUnorderedAction(LynxUIAction.UnorderedAction action) {
        Map<String, LynxUIAction.UnorderedAction> actions = mActionsMap.get(action.target());
        if (actions == null) {
            actions = new HashMap<>();
            mActionsMap.put(action.target(), actions);
        }
        actions.put(action.key(), action);
    }

    /* package */ boolean needDoActions() {
        return !(mActionsList.isEmpty() && mActionsMap.isEmpty());
    }

    /* package */  void doActions() {
        for (Map.Entry<RenderObjectImpl,  Map<String, LynxUIAction.UnorderedAction>> entry
                : mActionsMap.entrySet()) {

            Map<String, LynxUIAction.UnorderedAction> actions = entry.getValue();
            for (Map.Entry<String, LynxUIAction.UnorderedAction> entry2 : actions.entrySet()) {
                LynxUIAction action = entry2.getValue();
                action.doAction();
            }
        }

        for (int i = 0; i < mActionsList.size(); ++i) {
            LynxUIAction action = mActionsList.get(i);
            action.doAction();
        }

        mActionsMap.clear();
        mActionsList.clear();
    }

}
