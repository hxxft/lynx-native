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
        private Coalescence mDelegate;

        public OrderedAction(RenderObjectImpl target) {
            super(LynxUIActionType.DO_ORDERED_ACTION, target);
        }

        public OrderedAction(RenderObjectImpl target, Coalescence delegate) {
            super(LynxUIActionType.DO_ORDERED_ACTION, target);
            mDelegate = delegate;
        }

        @Override
        public boolean canCoalesce() {
            return mDelegate != null && mDelegate.canCoalesce();
        }

        @Override
        public int getCoalescingKey() {
            if (mDelegate != null) {
                return mDelegate.getCoalescingKey();
            }
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
