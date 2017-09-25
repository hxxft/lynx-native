// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.ui.recycler;

import com.lynx.core.impl.RenderObjectImpl;
import com.lynx.ui.LynxUIFactory;

import java.util.ArrayList;
import java.util.List;

public class RecyclableItemHelper {
    private List<RenderObjectImpl> mShadowNodes;
    private List<RecyclableItem> mRecyclableItems;

    public RecyclableItemHelper() {
        mShadowNodes = new ArrayList<>();
        mRecyclableItems = new ArrayList<>();
    }

    public void appendChildAt(RenderObjectImpl impl, int index) {
        mShadowNodes.add(index, impl);
        resetItemCount();
    }

    public RenderObjectImpl removeChildByIndex(int index) {
        RenderObjectImpl impl = mShadowNodes.remove(index);
        resetItemCount();
        return impl;
    }

    public RenderObjectImpl removeChild(RenderObjectImpl impl) {
        mShadowNodes.remove(impl);
        resetItemCount();
        return impl;
    }

    public void removeAll() {
        mShadowNodes.clear();
        resetItemCount();
    }

    private void resetItemCount() {
        mRecyclableItems.clear();
        for(int i = 0; i < mShadowNodes.size(); i++) {
            RenderObjectImpl impl = mShadowNodes.get(i);
            if(impl.getRenderObjectType() == LynxUIFactory.UI_TYPE_LIST_SHADOW) {
                for (int j = 0; j < impl.getChildCount(); j++) {
                    mRecyclableItems.add(
                            new RecyclableItem(impl.getChildAt(j), RecyclableState.Mercurial));
                }
            }else{
                mRecyclableItems.add(new RecyclableItem(impl, RecyclableState.Immutable));
            }
        }
    }

    public List<RenderObjectImpl> getShadowNodes() {
        return mShadowNodes;
    }

    public RenderObjectImpl getNode(int position) {
        if (position >= mRecyclableItems.size() || position < 0) {
            return null;
        }
        return mRecyclableItems.get(position).getRenderObjectImpl();
    }

    public int getItemCount() {
        return mRecyclableItems.size();
    }

    public RecyclableItem getItemAt(int location) {
        return mRecyclableItems.get(location);
    }

    public void notifyMercurialItemChanged() {
        resetItemCount();
    }
}
