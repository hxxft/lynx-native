// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.ui.swiper;

import android.content.Context;
import android.support.v4.view.PagerAdapter;
import android.view.View;
import android.view.ViewGroup;

import com.lynx.core.impl.RenderObjectImpl;
import com.lynx.ui.LynxUI;

import java.util.HashMap;
import java.util.Map;

public class SliderViewPagerAdapter extends PagerAdapter {

    protected BaseChildrenManager mChildrenManager;
    protected boolean mLooper = false;
    protected UIUsage mUIUsage;

    public SliderViewPagerAdapter(Context context) {
        mChildrenManager = new ChildrenManager(context);
        mUIUsage = new UIUsage();
    }

    public SliderViewPagerAdapter(BaseChildrenManager factory) {
        mChildrenManager = factory;
        mUIUsage = new UIUsage();
    }

    public void setLooper(boolean looper) {
        mLooper = looper;
        notifyDataSetChanged();
    }

    public void appendChild(int index, RenderObjectImpl child) {

        mChildrenManager.appendChild(index, child);
        notifyDataSetChanged();

    }

    public RenderObjectImpl removeChild(int index) {

        RenderObjectImpl removed = mChildrenManager.removeChild(index);
        notifyDataSetChanged();

        return removed;
    }

    @Override
    public void notifyDataSetChanged() {
        super.notifyDataSetChanged();
    }

    public void removeChild(RenderObjectImpl child) {

        mChildrenManager.removeChild(child);
        notifyDataSetChanged();
    }

    @Override
    public Object instantiateItem(ViewGroup container, int position) {
        LynxUI ui = mChildrenManager.createUI(position);
        mUIUsage.markOnScreen(ui);
        View view = ui.getView();
        if (view.getParent() != null) {
            ((ViewGroup) view.getParent()).removeView(view);
        }
        container.addView(view);
        return ui;
    }

    @Override
    public void destroyItem(ViewGroup container, int position, Object object) {
        LynxUI ui = (LynxUI) object;
        mUIUsage.markOffScreen(ui);
        if (mUIUsage.shouldRemove(ui)) {
            mChildrenManager.removeUI(ui);
            View view = ui.getView();
            container.removeView(view);
        }
    }

    @Override
    public int getCount() {
        if (mLooper && mChildrenManager.getChildCount() > 1) {
            return getActualChildCount() * 5;
        }
        return mChildrenManager.getChildCount();
    }

    @Override
    public int getItemPosition(Object object) {
        int index = mChildrenManager.indexOf(((LynxUI) object)
                .getRenderObjectImpl());
        return index < 0 ? POSITION_NONE : index;
    }

    @Override
    public boolean isViewFromObject(View view, Object object) {
        return view == ((LynxUI) object).getView();
    }

    public int getActualChildCount() {
        return mChildrenManager.getChildCount();
    }

    public void setRecyclerEnable(boolean enable) {
        if (!enable) {
            mChildrenManager.notUseRecycler();
        } else {
            mChildrenManager.useRecycler();
        }
    }

    private class UIUsage {
        Map<LynxUI, Integer> mRecorder;

        UIUsage() {
            mRecorder = new HashMap<>();
        }

        void markOnScreen(LynxUI ui) {
            int count = 0;
            if (mRecorder.containsKey(ui)) {
                count = mRecorder.get(ui);
            }
            mRecorder.put(ui, ++count);
        }

        void markOffScreen(LynxUI ui) {
            if (mRecorder.containsKey(ui)) {
                int count = mRecorder.get(ui);
                if (--count == 0) {
                    mRecorder.remove(ui);
                } else {
                    mRecorder.put(ui, count);
                }
            }
        }

        boolean shouldRemove(LynxUI ui) {
            return !mRecorder.containsKey(ui);
        }
    }
}
