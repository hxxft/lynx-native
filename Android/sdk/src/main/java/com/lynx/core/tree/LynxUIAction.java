// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.core.tree;

import com.lynx.core.base.Coalescence;
import com.lynx.core.impl.RenderObjectImpl;

public abstract class LynxUIAction {

    private LynxUIActionType mType;
    protected RenderObjectImpl mTarget;

    public LynxUIAction(LynxUIActionType type, RenderObjectImpl target) {
        mType = type;
        mTarget = target;
    }

    public LynxUIActionType type() {
        return mType;
    }

    public RenderObjectImpl target() {
        return mTarget;
    }

    public abstract void doAction();

    public static abstract class OrderedAction extends LynxUIAction implements Coalescence {

        public OrderedAction(RenderObjectImpl target) {
            super(LynxUIActionType.DO_ORDERED_ACTION, target);
        }

        @Override
        public boolean canCoalesce() {
            return false;
        }

        @Override
        public int getCoalescingKey() {
            return 0;
        }
    }

    public static abstract class UnorderedAction extends LynxUIAction {

        public UnorderedAction(RenderObjectImpl target) {
            super(LynxUIActionType.DO_UNORDERED_ACTION, target);
        }

        public abstract String key();
    }
}
