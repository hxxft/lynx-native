// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.ui.swiper;

import android.content.Context;
import android.support.v4.view.ViewPager;
import android.view.MotionEvent;

public abstract class SwiperView extends ViewPager {

    protected static final int NOT_SET = -1;

    protected SliderViewPagerAdapter mAdapter;
    protected LooperController mLooperController;

    protected boolean mIsAutoPlay = false;
    protected boolean mIsLooper = false;
    protected boolean mIsLooperState = false;
    protected boolean mIsAttachedToWindow = false;
    protected boolean mHasBeenStarted = false;

    private int mTargetItemWhenResetLoopState = NOT_SET;

    public SwiperView(Context context) {
        super(context);
        mAdapter = createSliderViewPagerAdapter(context);
        mLooperController = createLooperController();
        setAdapter(mAdapter);
        addOnPageChangeListener(new PositionController());
    }

    public abstract SliderViewPagerAdapter createSliderViewPagerAdapter(Context context);

    public abstract LooperController createLooperController();

    public void setLoopEnable(boolean enable) {
        if (enable) {
            enableLooper();
        } else {
            disableLooper();
        }
    }

    public void enableLooper() {
        mAdapter.setLooper(true);
        mIsLooper = true;
    }

    public void disableLooper() {
        clearLooperState();
        mAdapter.setLooper(false);
        mIsLooper = false;
    }

    @Override
    public boolean onTouchEvent(MotionEvent ev) {
        boolean handled = super.onTouchEvent(ev);
        if (ev.getAction() == MotionEvent.ACTION_CANCEL
                || ev.getAction() == MotionEvent.ACTION_UP) {
            mLooperController.resume();
        } else if (ev.getAction() == MotionEvent.ACTION_MOVE
                && handled
                && !mLooperController.isPause()
                && onInterceptTouchEvent(ev)) {
            mLooperController.pause();
        }
        return handled;
    }

    public void setDuration(int duration) {
        mLooperController.setDuration(duration);
    }

    public void setInterval(int interval) {
        mLooperController.setInterval(interval);
    }

    public void setAutoPlayEnable(boolean enable) {
        if (enable) {
            enableAutoPlay();
        } else {
            disableAutoPlay();
        }
    }

    public void enableAutoPlay() {
        mIsAutoPlay = true;
        startPlay();
    }

    public void disableAutoPlay() {
        mIsAutoPlay = false;
        stopPlay();
    }

    public void startPlay() {
        mHasBeenStarted = true;
        startPlayInner();
    }

    private void startPlayInner() {
        if (mIsAttachedToWindow
                && mAdapter.getActualChildCount() > 1
                && !mLooperController.isStart()) {
            mLooperController.start();
        }
    }

    public void stopPlay() {
        mHasBeenStarted = false;
        stopPlayInner();
    }

    private void stopPlayInner() {
        mLooperController.stop();
    }

    public void notifyDataSetChange() {
        if (mIsAutoPlay && mAdapter.getActualChildCount() > 1 && !mLooperController.isStart()) {
            startPlayInner();
        }
        if (mAdapter.getActualChildCount() <= 1 && mLooperController.isStart()) {
            stopPlayInner();
        }
        resetLooperState();
    }

    /**
     * 当初始化Item的looper状态的时候,设置初始的index
     */
    private void resetLooperState() {
        if (mAdapter.getActualChildCount() <= 1) {
            return;
        }
        if (!mIsLooper) {
            return;
        }
        mIsLooperState = true;

        int curItem = getLooperStartIndex(getCurrentItem());
        int remainder = 0;

        if (mAdapter.getActualChildCount() != 0) {
            remainder = curItem % mAdapter.getActualChildCount();
        }

        if (remainder != 0) {
            curItem -= remainder;
        }
        if (mIsAttachedToWindow) {
            setCurrentItem(curItem, false);
        } else {
            mTargetItemWhenResetLoopState = curItem;
        }
    }

    protected int getLooperStartIndex(int curItem) {
        int half = mAdapter.getCount() / 2;
        int factor = half / mAdapter.getActualChildCount();
        return factor * mAdapter.getActualChildCount() + curItem;
    }

    /**
     * 取消looper状态,将viewPager的index设置正确
     */
    private void clearLooperState() {

        if (!mIsLooper) {
            return;
        }

        mIsLooperState = false;
        int curItem = getCurrentItem();
        curItem %= mAdapter.getActualChildCount();
        setCurrentItem(curItem, false);
    }

    @Override
    protected void onAttachedToWindow() {
        super.onAttachedToWindow();
        mIsAttachedToWindow = true;

        if (mTargetItemWhenResetLoopState != NOT_SET) {
            setCurrentItem(mTargetItemWhenResetLoopState, false);
            mTargetItemWhenResetLoopState = NOT_SET;
        }

        if (mIsAutoPlay || mHasBeenStarted) {
            startPlayInner();
        }
    }

    @Override
    protected void onDetachedFromWindow() {
        super.onDetachedFromWindow();
        mIsAttachedToWindow = false;
        stopPlayInner();
    }

    @Override
    public SliderViewPagerAdapter getAdapter() {
        return mAdapter;
    }

    /**
     * Controller for page loop, while the position is near
     * the last or the first loop, reset current position to
     * the origin start position.
     */
    private class PositionController implements OnPageChangeListener {

        @Override
        public void onPageScrolled(int position, float positionOffset, int positionOffsetPixels) {
        }

        @Override
        public void onPageSelected(final int position) {
        }

        @Override
        public void onPageScrollStateChanged(int state) {
            if (mIsLooper
                    && state == SCROLL_STATE_IDLE
                    && (mAdapter.getCount() - getCurrentItem() <= mAdapter.getActualChildCount()
                    || getCurrentItem() < mAdapter.getActualChildCount())) {
                int targetPosition = getCurrentItem() % mAdapter.getActualChildCount();
                SwiperView.this.setCurrentItem(
                        getLooperStartIndex(targetPosition), false);
            }
        }
    }
}
