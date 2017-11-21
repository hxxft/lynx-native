// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.ui.swiper;

import android.view.ViewGroup;

import com.lynx.core.impl.RenderObjectImpl;
import com.lynx.ui.LynxUI;

public abstract class BaseChildrenManager {

    protected PagerRecyclerBin mRecyclerBin;
    protected boolean mNotUseRecycler = false;

    public BaseChildrenManager() {
        mRecyclerBin = new PagerRecyclerBin();
        mRecyclerBin.enableRecycle(true);
    }

    public void appendChild(int index, RenderObjectImpl child) {

    }

    public RenderObjectImpl removeChild(int index) {
        return null;
    }

    public void removeChild(RenderObjectImpl removed) {

    }

    /**
     * 与{@link SwiperViewPagerAdapter#instantiateItem(ViewGroup, int)} 同步使用
     * @param position
     * @return
     */
    public LynxUI createUI(int position) {
        return makeUI(position % getChildCount());
    }

    /**
     * 真正创建view的方法,需要复写
     * @param position
     * @return
     */
    public abstract LynxUI makeUI(int position);

    /**
     * 与{@link SwiperViewPagerAdapter#destroyItem(ViewGroup, int, Object)} 同步使用
     * @param ui
     * @return
     */
    public void removeUI(LynxUI ui) {
        if (!mNotUseRecycler && ui.getRenderObjectImpl() != null) {
            mRecyclerBin.recycleUI(ui.getRenderObjectImpl());
        }
    }

    public abstract int getChildCount();

    public abstract int indexOf(RenderObjectImpl impl);

    /**
     * 完全禁止回收功能
     */
    public void notUseRecycler() {
        mNotUseRecycler = true;
    }

    public void useRecycler() {
        mNotUseRecycler = false;
    }
}
