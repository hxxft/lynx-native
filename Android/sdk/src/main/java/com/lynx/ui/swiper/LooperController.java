// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.ui.swiper;

import android.animation.Animator;
import android.animation.ValueAnimator;
import android.os.Handler;
import android.os.Message;
import android.support.v4.view.ViewPager;
import android.view.animation.AccelerateInterpolator;

public class LooperController implements Runnable {

    private final static int DEFAULT_DURATION = 1000;
    private final static int DEFAULT_INTERVAL = 5000;

    private int mDuration = DEFAULT_DURATION;
    private int mInterval = DEFAULT_INTERVAL;
    private int oldDragPosition = 0;

    private SingleTaskTimer mTimer;
    private ViewPager mViewPager;
    private ValueAnimator mValueAnimator;

    private boolean mIsForward = true;

    public LooperController(ViewPager viewPager) {
        this(viewPager, DEFAULT_DURATION, DEFAULT_INTERVAL);
    }

    public LooperController(ViewPager viewPager, int duration, int interval) {
        this.mViewPager = viewPager;
        this.mDuration = duration;
        this.mInterval = interval;
        this.mTimer = new SingleTaskTimer();
        prepareAnimator();
    }

    public void start() {
        if (mTimer.isStop()) {
            mTimer.schedule(this, mInterval);
        }
    }

    public boolean isStart() {
        return !mTimer.isStop();
    }

    public boolean isPause() {
        return mTimer.isPause();
    }

    public void stop() {
        if (!mTimer.isStop()) {
            mTimer.stop();
            if (mValueAnimator != null && mValueAnimator.isRunning()) {
                mValueAnimator.cancel();
            }
        }
    }

    public void pause() {
        if (!mTimer.isPause()) {
            mTimer.pause();
            if (mValueAnimator != null && mValueAnimator.isRunning()) {
                mValueAnimator.cancel();
            }
        }
    }

    public void resume() {
        mTimer.resume();
    }

    public void setDuration(int duration) {
        mDuration = duration;
        mValueAnimator.setDuration(mDuration);
    }

    public void setInterval(int interval) {
        mInterval = interval;
    }

    protected void prepareAnimator() {

        mValueAnimator = ValueAnimator.ofInt(0, mViewPager.getWidth() - (mIsForward ? mViewPager.getPaddingLeft() : mViewPager.getPaddingRight()));

        mValueAnimator.addListener(mAnimatorListener);

        mValueAnimator.setInterpolator(mAccelerateInterpolator);

        mValueAnimator.addUpdateListener(mAnimatorUpdateListener);

        mValueAnimator.setDuration(mDuration);
    }

    @Override
    public void run() {
        mHandler.sendEmptyMessage(0x11);
    }

    private void animatePagerLoop() {
        mValueAnimator.setIntValues(0, mViewPager.getWidth()
                - mViewPager.getPaddingLeft() - mViewPager.getPaddingRight());
        oldDragPosition = 0;
        mValueAnimator.start();
    }

    private Handler mHandler = new Handler() {
        @Override
        public void handleMessage(Message msg) {
            if (!mTimer.isPause()) {
                animatePagerLoop();
            }
        }
    };

    private ValueAnimator.AnimatorUpdateListener mAnimatorUpdateListener = new ValueAnimator.AnimatorUpdateListener() {

        @Override
        public void onAnimationUpdate(ValueAnimator animation) {
            if (!mTimer.isPause()) {
                int dragPosition = (Integer) animation.getAnimatedValue();
                int dragOffset = dragPosition - oldDragPosition;
                oldDragPosition = dragPosition;

                if (mViewPager != null) {
                    int sx = mViewPager.getScrollX();
                    int sy = mViewPager.getScrollY();
                    mViewPager.scrollTo(sx + dragOffset, sy);
                }
            }
        }

    };

    private void checkCurrentPagePosition() {
        try {
            if (mViewPager != null) {
                // Ending with checking the position
                mViewPager.beginFakeDrag();
                mViewPager.endFakeDrag();
            }
        } catch (Exception e) {

        }
    }

    private AccelerateInterpolator mAccelerateInterpolator = new AccelerateInterpolator();

    private Animator.AnimatorListener mAnimatorListener = new Animator.AnimatorListener() {
        @Override
        public void onAnimationStart(Animator animation) {
        }

        @Override
        public void onAnimationEnd(Animator animation) {
            if (!mTimer.isPause()) {
                checkCurrentPagePosition();
                mTimer.schedule();
            }
        }

        @Override
        public void onAnimationCancel(Animator animation) {
        }

        @Override
        public void onAnimationRepeat(Animator animation) {
        }
    };

    // This timer only serve one runnable at a time. Not suitable for multiple runnable.
    class SingleTaskTimer implements Runnable {

        private Runnable mRunnable;
        private int mDuration = 0;
        private boolean mIsStop = true;
        private boolean mIsPause = true;
        private boolean mIsFinish = true;

        void stop() {
            mIsStop = true;
        }

        void pause() {
            mIsPause = true;
        }

        void resume() {
            mIsPause = false;
            if (mIsFinish && !mIsStop) {
                schedule();
            }
        }

        void schedule(Runnable runnable, int duration) {
            mIsStop = false;
            mIsPause = false;
            mRunnable = runnable;
            mDuration = duration;
            if (mIsFinish) {
                schedule();
            }
        }

        private void schedule() {

            if (mIsStop || mIsPause) {
                mIsFinish = true;
                return;
            }

            if (mRunnable == null) {
                throw new RuntimeException("Runnable should not be null.");
            }
            if (mDuration < 0) {
                throw new RuntimeException("Duration should not smaller than zero.");
            }
            mIsFinish = false;
            if (mViewPager != null) {
                mViewPager.postDelayed(this, mDuration);
            }
        }

        @Override
        public void run() {

            if (mIsStop || mIsPause) {
                mIsFinish = true;
                return;
            }

            mRunnable.run();
        }

        public boolean isStop() {
            return mIsStop;
        }

        public boolean isPause() {
            return mIsPause;
        }

    }
}
