// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.ui.recycler;

import android.content.Context;
import android.util.SparseArray;
import android.widget.AdapterView;

import com.lynx.base.Style;
import com.lynx.core.impl.RenderObjectImpl;
import com.lynx.ui.LynxUI;
import com.lynx.ui.LynxUIFactory;
import com.lynx.ui.LynxUIGroup;

import java.util.ArrayList;
import java.util.List;

/**
 * create and save views until the view pool has enough views for a whole screen to use
 */
public class RecycledUIPool {
    private static final int MAX_DELLOCATE_NUM = 3;
    private static final int MAX_CACHE_NUM = 1;

    private List<RenderObjectImpl> mCachedRenderObjectImplList;
    // collect those View that outside screen and ready to be reused
    private SparseArray<List<LynxUI>> mUIPool;

    private boolean mIsHiSpeed = false;
    private Context mContext;

    public RecycledUIPool(Context context) {
        mContext = context;
        mUIPool = new SparseArray<>();
        mCachedRenderObjectImplList = new ArrayList<>();
    }

    public LynxUI getUI(int position, RenderObjectImpl node, boolean isHiSpeed) {
        mIsHiSpeed = isHiSpeed;
        LynxUI ui = createViewInternal(position, node);
        return ui;
    }

    // Check which kind of node should not be handled by ImplPoolManager
    private boolean shouldBeHandled(RenderObjectImpl impl) {
        Style style = impl.getStyle();
        if (style != null && style.mPositionType == Style.CSS_POSITION_FIXED) {
            return false;
        }
        return true;
    }

    private LynxUI createViewInternal(int position, RenderObjectImpl renderObjectImpl) {

        if (!shouldBeHandled(renderObjectImpl)) {
            return null;
        }

        if (renderObjectImpl.getUI() != null) {
            LynxUI ui = renderObjectImpl.getUI();
            return ui;
        }

        LynxUI ui;
        List<LynxUI> list = mUIPool.get(renderObjectImpl.getRenderObjectType());

        if (list == null || list.isEmpty()) {

            ui = LynxUIFactory.create(mContext, renderObjectImpl);

        } else {

            ui = list.remove(list.size() - 1);
            if (ui instanceof HiSpeedStopLoadItem) {
                if (mIsHiSpeed) {
                    ((HiSpeedStopLoadItem) ui).stopLoadResWhenViewInHiSpeed();
                } else {
                    ((HiSpeedStopLoadItem) ui).allowLoadResWhenViewInHiSpeed();
                }
            }

            ui.bindData(renderObjectImpl);

        }

        if (isContainer(ui)) {
            buildChildren(position, renderObjectImpl);
        }

        return ui;
    }

    protected void buildChildren(int position, RenderObjectImpl parentImpl) {

        for (int i = 0; i < parentImpl.getChildCount(); i++) {
            RenderObjectImpl child = parentImpl.getChildAt(i);
            // Do not create child if the container has function of recycling child
            if (!isRecyclerContainer(parentImpl.getUI())) {
                LynxUI impl = createViewInternal(position, child);
                if (impl == null) continue;
                if (impl.getView().getParent() != null) continue;
            }
            parentImpl.getUI().insertChild(child, i);
        }
    }

    public void moveToCache(RenderObjectImpl impl) {
        if (impl == null) {
            return;
        }

        if (impl.getUI().getView().getParent() != null) {
            return;
        }

        if (impl.getUI().getView().getParent() != null
                && impl.getUI().getView().getParent() instanceof AdapterView) {
            return;
        }

        moveToCacheInternal(impl);
    }

    private void moveToCacheInternal(RenderObjectImpl impl) {
        if (mCachedRenderObjectImplList.contains(impl)) {
            return;
        }

        mCachedRenderObjectImplList.add(impl);
        for (int i = 0; i < MAX_DELLOCATE_NUM; i++) {
            if (mCachedRenderObjectImplList.size() >= MAX_CACHE_NUM) {
                recycle(mCachedRenderObjectImplList.remove(0));
            } else {
                break;
            }
        }
    }

    private void recycle(RenderObjectImpl renderObjectImpl) {
        if (renderObjectImpl == null || !shouldBeHandled(renderObjectImpl)) {
            return;
        }

        LynxUI ui = renderObjectImpl.getUI();

        if (ui == null) {
            return;
        }

        if (renderObjectImpl.getChildCount() > 0) {
            for (int i = 0; i < renderObjectImpl.getChildCount(); i++) {
                ui.removeChild(renderObjectImpl.getChildAt(i));
                recycle(renderObjectImpl.getChildAt(i));
            }
        }

        // Firstly get view's type
        int type = renderObjectImpl.getRenderObjectType();

        // Unbind data from view
        ui.unbindData();

        // Add the view to pool
        List<LynxUI> list = mUIPool.get(type);
        if (list == null) {
            list = new ArrayList<>();
        }
        list.add(ui);
        mUIPool.put(type, list);
    }

    private boolean isContainer(LynxUI impl) {
        return impl instanceof LynxUIGroup;
    }

    private boolean isRecyclerContainer(LynxUI impl) {
        return impl instanceof ILynxUIRecycler;
    }
}
