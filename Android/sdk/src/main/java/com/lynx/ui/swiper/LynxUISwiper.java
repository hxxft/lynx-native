// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.ui.swiper;

import android.content.Context;
import android.support.v4.view.ViewPager;
import android.view.ViewGroup;

import com.lynx.core.base.LynxEvent;
import com.lynx.core.impl.RenderObjectImpl;
import com.lynx.ui.LynxUIGroup;

public class LynxUISwiper<T extends AndroidSwiper> extends LynxUIGroup<T> {

    private static final String EVENT_PAGE_CHANGE = "change";
    private static final String ATTR_AUTO_PLAY = "auto-play";
    private static final String ATTR_LOOP = "loop";
    private static final String ATTR_CURRENT = "current";
    private static final String ATTR_DURATION = "duration";
    private static final String ATTR_INTERVAL = "interval";

    private boolean mIsFirstInit = true;
    private ViewPager.OnPageChangeListener mOnPageChangeListener;

    public LynxUISwiper(Context context, RenderObjectImpl node) {
        super(context, node);
    }

    @Override
    protected T createView(Context context) {
        return (T) new AndroidSwiper(context, this);
    }

    @Override
    public void setAttribute(String key, String value) {
        super.setAttribute(key, value);
        switch (key) {
            case ATTR_AUTO_PLAY:
                mView.setAutoPlayEnable(Boolean.valueOf(value));
                break;
            case ATTR_LOOP:
                mView.setLoopEnable(Boolean.valueOf(value));
                break;
            case ATTR_CURRENT:
                mView.setCurrentItem(Integer.valueOf(value));
                break;
            case ATTR_INTERVAL:
                mView.setInterval(Integer.valueOf(value));
                break;
            case ATTR_DURATION:
                mView.setDuration(Integer.valueOf(value));
                break;
            default:
                break;
        }
    }

    @Override
    public void addEventListener(String event) {
        super.addEventListener(event);
        if (event.equals(EVENT_PAGE_CHANGE)) {
            if (mOnPageChangeListener == null) {
                mOnPageChangeListener = new OnSliderViewPageChangeListener();
            }
            mView.addOnPageChangeListener(mOnPageChangeListener);
        }
    }

    @Override
    public void removeEventListener(String event) {
        super.removeEventListener(event);
        if (event.equals(EVENT_PAGE_CHANGE)) {
            mView.removeOnPageChangeListener(mOnPageChangeListener);
        }
    }

    @Override
    public void insertChild(RenderObjectImpl child, int index) {
        mView.mAdapter.appendChild(index, child);
        if (mView.mIsAutoPlay
                && mRenderObjectImpl.getChildCount() > 1
                && !mView.mLooperController.isStart()) {
            mView.mLooperController.start();
        }
    }

    @Override
    public void removeChild(RenderObjectImpl child) {
        if (mRenderObjectImpl.getChildCount() < 2) {
            mView.mLooperController.stop();
        }
        mView.mAdapter.removeChild(child);
    }

    @Override
    public ViewGroup.LayoutParams generateLayoutParams(ViewGroup.LayoutParams childParams) {
        if (childParams instanceof ViewPager.LayoutParams) {
            return childParams;
        }
        return new ViewPager.LayoutParams();
    }

    @Override
    public void layoutChildren() {
        layoutGrandChildren();
    }

    protected void layoutGrandChildren() {
        for (int i = 0; i < mRenderObjectImpl.getChildCount(); ++i) {
            RenderObjectImpl child = mRenderObjectImpl.getChildAt(i);
            if (child.hasUI()
                    && child.getUI() instanceof LynxUIGroup) {
                ((LynxUIGroup) mRenderObjectImpl.getChildAt(i).getUI()).layoutChildren();
            }
        }
    }

    class OnSliderViewPageChangeListener implements ViewPager.OnPageChangeListener {

        @Override
        public void onPageScrolled(int position, float positionOffset, int positionOffsetPixels) {
            // fix onPageSelected is not be called when viewpager first page init
            if (mIsFirstInit
                    && position == 0
                    && positionOffset == 0
                    && positionOffsetPixels == 0
                    && mView.mAdapter.getActualChildCount() > 0){
                onPageSelected(0);
                mIsFirstInit = false;
            }
        }

        @Override
        public void onPageSelected(int position) {
            if (mRenderObjectImpl != null && mView.mAdapter.getActualChildCount() != 0) {
                int i = position % mView.mAdapter.getActualChildCount();
                LynxEvent event = new LynxEvent(EVENT_PAGE_CHANGE);
                event.setProperty("index", i);
                postEvent(EVENT_PAGE_CHANGE, event);
            }
        }

        @Override
        public void onPageScrollStateChanged(int state) {

        }
    }
}
