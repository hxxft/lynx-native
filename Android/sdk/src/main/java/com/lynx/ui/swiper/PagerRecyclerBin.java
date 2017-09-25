// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.ui.swiper;

import android.util.SparseArray;

import com.lynx.core.impl.RenderObjectImpl;
import com.lynx.ui.LynxUI;

import java.util.ArrayList;
import java.util.List;

public class PagerRecyclerBin {

    private SparseArray<List<LynxUI>> mRecycledUIBin;

    private boolean enableRecycle = false;

    public PagerRecyclerBin() {
        mRecycledUIBin = new SparseArray<>();
    }

    /**
     * 获取被回收的view,可以进行重复使用
     * @param type
     * @return
     */
    public LynxUI getRecycledUI(int type) {

        List<LynxUI> list = mRecycledUIBin.get(type);

        if (list != null && list.size() > 0) {
            return list.remove(0);
        }
        return null;
    }

    public void recycleUI(RenderObjectImpl impl) {
        if (!enableRecycle) {
            return;
        }

        LynxUI ui = impl.getUI();

        if (ui == null) {
            return;
        }

        for (int i = 0; i < impl.getChildCount(); i++) {
            ui.removeChild(impl.getChildAt(i));
            recycleUI(impl.getChildAt(i));
        }

        // firstly get view's type
        int type = impl.getRenderObjectType();

        // unbind data from view
        ui.unbindData();

        // add the view to pool
        List<LynxUI> list = mRecycledUIBin.get(type);

        if (list == null) {
            list = new ArrayList<>();
            mRecycledUIBin.put(type, list);
        }
        list.add(ui);
        mRecycledUIBin.put(type, list);
    }

    public void enableRecycle(boolean enable) {
        enableRecycle = enable;
    }

    public void clear() {
        mRecycledUIBin.clear();
    }

}
