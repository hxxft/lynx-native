// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.ui.scroll;

import android.content.Context;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.widget.HorizontalScrollView;
import android.widget.LinearLayout;
import android.widget.ScrollView;

import com.lynx.base.Size;
import com.lynx.ui.LynxUI;

public class AndroidScrollView extends ScrollView {
    public static final int HORIZONTAL = 0;
    public static final int VERTICAL = 1;
    private LinearLayout mLinearLayout;
    private boolean isLinearLayoutExist = false;
    private HorizontalScrollView mHorizontalScrollView;
    private int mLastScrollY;
    private int mLastScrollX;

    boolean isHorizontal = false;
    private Size mMeasureSize = new Size();
    private boolean isMotionDown = false;
    private boolean isScrollStart = false;
    private OnScrollListener mOnScrollListener;

    public AndroidScrollView(Context context, LynxUI ui) {
        super(context);
        init();
        createInternalLinearLayout();
    }

    protected void init() {
        this.setVerticalScrollBarEnabled(false);
        this.setOverScrollMode(HorizontalScrollView.OVER_SCROLL_NEVER);
        this.setFadingEdgeLength(0);
        this.setScrollContainer(false);
    }

    @Override
    public void addView(View child) {
        if (isLinearLayoutExist) {
            mLinearLayout.addView(child);
        } else {
            super.addView(child);
            isLinearLayoutExist = true;
        }
    }

    @Override
    public void addView(View child, int index) {
        if (isLinearLayoutExist) {
            mLinearLayout.addView(child, index);
        } else {
            super.addView(child, index);
            isLinearLayoutExist = true;
        }
    }

    @Override
    public void addView(View child, int index, ViewGroup.LayoutParams params) {
        if (isLinearLayoutExist) {
            mLinearLayout.addView(child, index, params);
        } else {
            super.addView(child, index, params);
            isLinearLayoutExist = true;
        }
    }

    @Override
    public void addView(View child, ViewGroup.LayoutParams params) {
        if (isLinearLayoutExist) {
            mLinearLayout.addView(child, params);
        } else {
            super.addView(child, params);
            isLinearLayoutExist = true;
        }
    }

    @Override
    public void addView(View child, int width, int height) {
        if (isLinearLayoutExist) {
            mLinearLayout.addView(child, width, height);
        } else {
            super.addView(child, width, height);
            isLinearLayoutExist = true;
        }
    }

    @Override
    public void removeView(View view) {
        if (isLinearLayoutExist) {
            mLinearLayout.removeView(view);
        } else {
            super.removeView(view);
            isLinearLayoutExist = true;
        }
    }

    @Override
    public void removeViewAt(int index) {
        if (isLinearLayoutExist) {
            mLinearLayout.removeViewAt(index);
        } else {
            super.removeViewAt(index);
            isLinearLayoutExist = true;
        }
    }

    @Override
    public void removeAllViews() {
        if (isLinearLayoutExist) {
            mLinearLayout.removeAllViews();
        } else {
            super.removeAllViews();
            isLinearLayoutExist = true;
        }
    }

    @Override
    protected void onScrollChanged(int l, int t, int oldl, int oldt) {
        super.onScrollChanged(l, t, oldl, oldt);
        if (isMotionDown && !isScrollStart) {
            isScrollStart = true;
            if (mOnScrollListener != null) {
                mOnScrollListener.onScrollStart();
            }
        } else {
            if (mOnScrollListener != null) {
                mOnScrollListener.onScrollChanged(mLastScrollX, t, mLastScrollX, oldt);
            }
        }
        if (mLastScrollY != this.getScrollY()) {
            mLastScrollY = this.getScrollY();
        }
    }

    @Override
    public boolean onTouchEvent(MotionEvent event) {
        analyseMotion(event);
        return super.onTouchEvent(event);
    }

    @Override
    public boolean onInterceptTouchEvent(MotionEvent ev) {
        if (!isHorizontal) {
            return super.onInterceptTouchEvent(ev);
        } else {
            return false;
        }
    }

    public void setOnScrollListener(OnScrollListener listener) {
        mOnScrollListener = listener;
    }

    private void analyseMotion(MotionEvent event) {
        if (event.getAction() == MotionEvent.ACTION_DOWN) {
            isMotionDown = true;
        } else if (event.getAction() == MotionEvent.ACTION_UP) {
            if (isScrollStart && mOnScrollListener != null) {
                mOnScrollListener.onScrollCancel();
            }
            isScrollStart = false;
            isMotionDown = false;
        }
    }

    public void setScrollTo(int x, int y, boolean animate) {
        if (animate) {
            if (isHorizontal) {
                mHorizontalScrollView.smoothScrollTo(x, y);
            } else {
                this.smoothScrollTo(x, y);
            }
        } else {
            if (isHorizontal) {
                mHorizontalScrollView.scrollTo(x, y);
            } else {
                this.scrollTo(x, y);
            }
        }
    }

    public void setOrientation(int orientation) {
        if (orientation == HORIZONTAL) {
            mLinearLayout.setOrientation(LinearLayout.HORIZONTAL);
            isHorizontal = true;
        } else if (orientation == VERTICAL) {
            mLinearLayout.setOrientation(LinearLayout.VERTICAL);
            isHorizontal = false;
        }
    }

    private void createInnerComponent() {
        mLinearLayout = new LinearLayout(this.getContext()) {

            @Override
            protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
                setMeasuredDimension((int) mMeasureSize.mWidth, (int) mMeasureSize.mHeight);
            }

            @Override
            protected void onLayout(boolean changed, int l, int t, int r, int b) {
            }

        };
        mLinearLayout.setOrientation(LinearLayout.VERTICAL);
    }

    private void createInnerScrollView() {
        mHorizontalScrollView = new HorizontalScrollView(this.getContext()) {

            @Override
            protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
                super.onMeasure(widthMeasureSpec, heightMeasureSpec);
            }

            @Override
            public boolean onInterceptTouchEvent(MotionEvent ev) {
                if (isHorizontal) {
                    return super.onInterceptTouchEvent(ev);
                } else {
                    return false;
                }
            }

            @Override
            public boolean onTouchEvent(MotionEvent ev) {
                analyseMotion(ev);
                if (isHorizontal) {
                    return super.onTouchEvent(ev);
                } else {
                    return false;
                }
            }

            @Override
            protected void onScrollChanged(int l, int t, int oldl, int oldt) {
                super.onScrollChanged(l, t, oldl, oldt);
                if (isMotionDown && !isScrollStart) {
                    isScrollStart = true;
                    if (mOnScrollListener != null) {
                        mOnScrollListener.onScrollStart();
                    }
                } else {
                    if (mOnScrollListener != null) {
                        mOnScrollListener.onScrollChanged(l, t, oldl, oldt);
                    }
                }
                if (mLastScrollX != this.getScrollX()) {
                    mLastScrollX = this.getScrollX();
                }
            }

        };
        mHorizontalScrollView.setHorizontalScrollBarEnabled(false);
        mHorizontalScrollView.setOverScrollMode(HorizontalScrollView.OVER_SCROLL_NEVER);
        mHorizontalScrollView.setFadingEdgeLength(0);
    }

    private void createInternalLinearLayout() {
        if (mLinearLayout == null) {
            createInnerComponent();
            createInnerScrollView();
            mHorizontalScrollView.addView(mLinearLayout,
                    new LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT));
            this.addView(mHorizontalScrollView,
                    new LayoutParams(LayoutParams.WRAP_CONTENT, LayoutParams.WRAP_CONTENT));
        }
    }

    public void setMeasuredSize(Size size) {
        mMeasureSize = size;
    }

    public HorizontalScrollView getHScrollView() {
        return mHorizontalScrollView;
    }

    public LinearLayout getLinearLayout() {
        return mLinearLayout;
    }

    public interface OnScrollListener {
        void onScrollStop();

        void onScrollChanged(int l, int t, int oldl, int oldt);

        void onScrollStart();

        void onScrollCancel();
    }
}
