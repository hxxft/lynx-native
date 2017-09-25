// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.ui.recycler;

import android.content.Context;
import android.widget.LinearLayout;

import com.lynx.core.impl.RenderObjectImpl;
import com.lynx.ui.LynxUI;
import com.lynx.ui.LynxUIGroup;

public class Recycler {

    public static final int SCROLL_STATE_IDLE = 0;
    public static final int SCROLL_STATE_OTHER = 0;
    public static final int VERTICAL = LinearLayout.VERTICAL;
    public static final int HORIZONTAL = LinearLayout.HORIZONTAL;

    private int mCurAxisStart = 0;
    private int mCurAxisEnd = 0;
    private int mPreFirstItem = -1;
    private int mOrientation = VERTICAL;

    private RecycledUIPool mRecycledUIPool;
    private RecyclableItemHelper mItemHelper;

    // 前一个scrollChange事件触发时间
    private long mPreviousEventTime = 0;
    // 当前滑动速度（当前1s内滑动多少个iterm)
    private int mScrollSpeed = 0;
    // 在一个页面内最多同时展示的Item个数
    private int mMaxShowItemCounts = 0;
    // 基准判断的最多同时展示item个数
    private final static double STANDARD_MAX_SHOW_ITEM = 3d;
    // 页面内最多同时展示的item个数与基准的比例
    private double mMaxShowItemRatio = 1d;
    // 速度阈值，当超过了这个阈值，则不进行资源的加载
    private final static int SPEED_THRESHOLD = 15;
    // 判断是否是高速滑动状态
    private boolean mIsHiSpeed = false;
    private boolean mHasScrollItemChanged = false;

    public Recycler(Context context, RecyclableItemHelper itemHelper) {
        mRecycledUIPool = new RecycledUIPool(context);
        mItemHelper = itemHelper;
    }

    public void setOrientation(int orientation) {
        mOrientation = orientation;
    }

    /**
     * Get view from pool, if there no available view in pool, it will create a new one.
     * @param viewType
     * @return
     */
    public LynxUI getRecyclerUI(int viewType) {

        RecyclableItem childView = mItemHelper.getItemAt(viewType);

        boolean isImmutable = childView.getState() == RecyclableState.Immutable;

        if (isImmutable && childView.getRenderObjectImpl().getUI() != null) {
            return childView.getRenderObjectImpl().getUI();
        }

        // Immutable node does not concern about speed.
        return mRecycledUIPool.getUI(
                viewType,
                childView.getRenderObjectImpl(),
                !isImmutable && mIsHiSpeed);
    }

    public void onScrollItemChange(int curFirstItem, int curLastItem) {

        if (curFirstItem == mPreFirstItem) {
            return;
        }
        //当curFirstItem 大于 孩子的数目时，不执行。
        //问题出现于：没有孩子的情况中，当获取了 位置0的view时，mChildViews的长度为0，出现crash。
        if (curFirstItem >= mItemHelper.getItemCount()){
            return;
        }
        mHasScrollItemChanged = true;
        // 计算滑动时1s时间内滑动了多少个item
        calculateCurSpeed(curFirstItem);

        mPreFirstItem = curFirstItem;
        setChildViewPosition();
        if (curLastItem < 0 || curLastItem >= mItemHelper.getItemCount()) {
            return;
        }
        mCurAxisStart = mOrientation == VERTICAL ?
                mItemHelper.getItemAt(curFirstItem).getTop()
                : mItemHelper.getItemAt(curFirstItem).getLeft();
        mCurAxisEnd = mOrientation == VERTICAL ?
                mItemHelper.getItemAt(curFirstItem).getBottom()
                : mItemHelper.getItemAt(curFirstItem).getRight();
        addToRecyclerIfCan();
    }

    /**
     * 将超出屏幕外的view进行回收
     */
    protected void addToRecyclerIfCan() {

        for (int i = 0; i < mItemHelper.getItemCount(); i++) {

            RecyclableItem child = mItemHelper.getItemAt(i);

            if (child.getState() == RecyclableState.Immutable) {
                continue;
            }

            LynxUI impl = child.getRenderObjectImpl().getUI();
            if (impl == null) {
                continue;
            }

            int childActualAxisStart = mOrientation == VERTICAL ?
                    child.getTop() : child.getLeft();
            int childActualAxisEnd = mOrientation == VERTICAL ?
                    child.getBottom() : child.getRight();
            if ((childActualAxisEnd > mCurAxisStart && childActualAxisStart <= mCurAxisStart)
                    || (childActualAxisEnd <= mCurAxisEnd && childActualAxisStart >= mCurAxisStart)
                    || (childActualAxisEnd >= mCurAxisEnd && childActualAxisStart < mCurAxisEnd)) {
                continue;
            }

            mRecycledUIPool.moveToCache(child.getRenderObjectImpl());
        }
    }

    private void setChildViewPosition() {
        int scrollY = 0;
        int scrollX = 0;
        for (int i = 0; i < mItemHelper.getItemCount(); ++i) {
            RenderObjectImpl child = mItemHelper.getNode(i);
            if (child != null) {
                mItemHelper.getItemAt(i).updatePosition(scrollY, scrollX);
                scrollY += mOrientation == VERTICAL ? child.getPosition().getHeight() : 0;
                scrollX += mOrientation == VERTICAL ? 0 : child.getPosition().getWidth();
            }
        }
    }

    /**
     * 重置滑动速度
     */
    private void resetSpeed() {
        mScrollSpeed = 0;
        mIsHiSpeed = false;
    }

    /**
     * 强制加载正在展示的item的图片
     */
    void forceInvalidate() {
        int maxCount = 0;
        for (int i = 0; i < mItemHelper.getItemCount(); i++) {

            RecyclableItem child = mItemHelper.getItemAt(i);

            if (child.getState() == RecyclableState.Immutable) {
                continue;
            }

            LynxUI ui = child.getRenderObjectImpl().getUI();
            if (ui == null) {
                continue;
            }
            maxCount++;
            reloadItemRes(child.getRenderObjectImpl());
        }
        mMaxShowItemCounts = Math.max(mMaxShowItemCounts, maxCount);
        mMaxShowItemRatio = mMaxShowItemCounts / STANDARD_MAX_SHOW_ITEM;
    }

    /**
     * 对象进行重新加载资源
     * @param node
     */
    void reloadItemRes(RenderObjectImpl node) {

        for (int i = 0; i < node.getChildCount(); i++) {
            RenderObjectImpl child = node.getChildAt(i);
            LynxUI ui = child.getUI();

            if (ui instanceof HiSpeedStopLoadItem) {
                ((HiSpeedStopLoadItem) ui).reloadResWhenSuitable();
            }

            if (ui instanceof LynxUIGroup) {
                reloadItemRes(child);
            }
        }
    }

    public void onScrollStateChanged(int scrollState) {
        // 在停止的时候进行资源的重新加载
        if (scrollState == SCROLL_STATE_IDLE) {
            if (mHasScrollItemChanged) {
                resetSpeed();
                forceInvalidate();
            }
            mHasScrollItemChanged = false;
        }
    }

    /**
     * 计算当前滑动的速度，并判断是否是高速状态
     * @param curFirstItem
     */
    protected void calculateCurSpeed(int curFirstItem) {
        if (mPreFirstItem != curFirstItem && mMaxShowItemCounts != 0){
            long currTime = System.currentTimeMillis();
            long timeToScrollOneElement = currTime - mPreviousEventTime;
            if (timeToScrollOneElement == 0) {
                timeToScrollOneElement = 1;
            }
            if (mMaxShowItemRatio == 0) {
                mMaxShowItemRatio = 1d;
            }
            mScrollSpeed = (int) ((1000 / timeToScrollOneElement) / mMaxShowItemRatio);
            mPreFirstItem = curFirstItem;
            mPreviousEventTime = currTime;
            mIsHiSpeed = (mScrollSpeed >= SPEED_THRESHOLD);
        }
    }

}
