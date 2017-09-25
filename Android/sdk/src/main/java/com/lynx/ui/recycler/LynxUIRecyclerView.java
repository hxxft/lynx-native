// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.ui.recycler;

import android.content.Context;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.util.Log;
import android.view.View;
import android.view.ViewGroup;

import com.lynx.base.Position;
import com.lynx.base.Style;
import com.lynx.core.base.LynxEvent;
import com.lynx.core.impl.RenderObjectAttr;
import com.lynx.core.impl.RenderObjectImpl;
import com.lynx.ui.LynxUIFactory;
import com.lynx.ui.LynxUIGroup;

public class LynxUIRecyclerView
        extends LynxUIGroup<AndroidRecyclerView>
        implements ILynxUIRecycler {

    public final static String SCROLL_EVENT_TAG = "scroll";
    public static int VERTICAL = Recycler.VERTICAL;
    public static int HORIZONTAL = Recycler.HORIZONTAL;

    private RecyclerViewAdapter mAdapter;
    private LinearLayoutManager mLinearLayoutManager;
    private RecyclableItemHelper mItemHelper;
    private Recycler mRecycler;

    private int mScrollTop = 0;
    private int mScrollLeft = 0;
    private int mLastScrollTop = 0;
    private int mLastScrollLeft = 0;

    public LynxUIRecyclerView(Context context, RenderObjectImpl impl) {
        super(context, impl);
    }

    @Override
    protected AndroidRecyclerView createView(Context context) {

        AndroidRecyclerView recyclerView = new AndroidRecyclerView(context, this);
        // Remove over scroll effect
        recyclerView.setOverScrollMode(RecyclerView.OVER_SCROLL_NEVER);
        recyclerView.setFadingEdgeLength(0);

        mItemHelper = new RecyclableItemHelper();
        // Create recycler
        mRecycler = new Recycler(context, mItemHelper);
        // Create adapter
        mAdapter = new RecyclerViewAdapter(mRecycler, mItemHelper);
        // Create layout manager
        mLinearLayoutManager = new LinearLayoutManager(context);

        // Default orientation
        setOrientation(VERTICAL);

        recyclerView.setLayoutManager(mLinearLayoutManager);
        recyclerView.setAdapter(mAdapter);
        recyclerView.addOnScrollListener(new RecyclerViewOnScrollListener());
        // Stop recyclerView from recycling
        recyclerView.setRecycledViewPool(new RecyclerView.RecycledViewPool() {
            @Override
            public void putRecycledView(RecyclerView.ViewHolder scrap) {}
            @Override
            public RecyclerView.ViewHolder getRecycledView(int viewType) {
                return null;
            }
        });
        return recyclerView;
    }

    @Override
    public void insertChild(RenderObjectImpl child, int index) {
        if(index <= mItemHelper.getShadowNodes().size()) {
            mItemHelper.appendChildAt(child,
                    index < 0 ? mItemHelper.getItemCount() : index);
            mAdapter.notifyDataSetChanged();
        }else {
            Log.e("Lynx Error", "appendChildAt: " + index + " out of range!!!");
        }
    }

    @Override
    public void removeChild(RenderObjectImpl child) {
        mItemHelper.removeChild(child);
        mAdapter.notifyDataSetChanged();
    }

    @Override
    public ViewGroup.LayoutParams generateLayoutParams(ViewGroup.LayoutParams childParams) {
        if (childParams instanceof RecyclerView.LayoutParams) {
            return childParams;
        }
        return new RecyclerView.LayoutParams(0, 0);
    }

    @Override
    public void bindData(RenderObjectImpl impl) {
        super.bindData(impl);
        mView.setAdapter(mAdapter);
    }

    @Override
    public void unbindData() {
        super.unbindData();
        mView.setAdapter(null);
    }

    @Override
    public void updateStyle(Style style) {
        super.updateStyle(style);
        setOrientation(style == null || style.mFlexDirection == Style.CSSFLEX_DIRECTION_COLUMN ?
                VERTICAL : HORIZONTAL);
    }

    @Override
    public void notifyChildrenChanged() {
        mItemHelper.notifyMercurialItemChanged();
        mAdapter.notifyDataSetChanged();
    }

    @Override
    public void measureChildren() {
        for (int i = 0; i < mRenderObjectImpl.getChildCount(); ++i) {
            if (mRenderObjectImpl.getChildAt(i).hasUI()) {
                mRenderObjectImpl.getChildAt(i).getUI().measure();
            }
            if (mRenderObjectImpl.getChildAt(i).getRenderObjectType() == 5) {
                for (int j = 0; j < mRenderObjectImpl.getChildAt(i).getChildCount(); j++) {
                    if (mRenderObjectImpl.getChildAt(i).getChildAt(j).hasUI()) {
                        mRenderObjectImpl.getChildAt(i).getChildAt(j).getUI().measure();
                    }
                }
            }
        }
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
            if (child.getRenderObjectType() == LynxUIFactory.UI_TYPE_LIST_SHADOW) {
                for (int j = 0; j < child.getChildCount(); j++) {
                    RenderObjectImpl grandChild = child.getChildAt(j);
                    if (grandChild.hasUI()
                            && grandChild.getUI() instanceof LynxUIGroup) {
                        ((LynxUIGroup) grandChild.getUI()).layoutChildren();
                    }
                }
            }
        }
    }

    @Override
    public void setPosition(Position position) {
        super.setPosition(position);
        // TODO: 17/8/11 If adds or removes views does not cause position or
        // size changed, it may not call this method which will not notify
        // dataSetChanged.
        mItemHelper.notifyMercurialItemChanged();
        mAdapter.notifyDataSetChanged();
    }

    @Override
    public void setData(int attr, Object param) {
        super.setData(attr, param);
        if (attr == RenderObjectAttr.SCROLL_LEFT.value()) {
            scrollLeft((Integer) param);
        } else if (attr == RenderObjectAttr.SCROLL_TOP.value()) {
            scrollTop((Integer) param);
        }
    }

    public void setOrientation(int orientation) {
        if (orientation == HORIZONTAL) {
            mRecycler.setOrientation(HORIZONTAL);
            mLinearLayoutManager.setOrientation(LinearLayoutManager.HORIZONTAL);
        } else {
            mRecycler.setOrientation(VERTICAL);
            mLinearLayoutManager.setOrientation(LinearLayoutManager.VERTICAL);
        }
    }

    protected void obtainScrollInfo() {
        View view = mView.getChildAt(0); //this is the first visible row
        if(view == null) {
            return;
        }

        if (mLinearLayoutManager.getOrientation() == LinearLayoutManager.VERTICAL) {
            mScrollTop = -view.getTop();
            int firstVisiblePosition = mLinearLayoutManager.findFirstVisibleItemPosition();
            for (int i = 0; i < firstVisiblePosition; i++) {
                RenderObjectImpl child = mItemHelper.getNode(i);
                if (child != null) {
                    mScrollTop += child.getPosition().getHeight();
                }
            }
        } else {
            mScrollLeft = -view.getLeft();
            int firstVisiblePosition = mLinearLayoutManager.findFirstVisibleItemPosition();
            for (int i = 0; i < firstVisiblePosition; i++) {
                RenderObjectImpl child = mItemHelper.getNode(i);
                if (child != null) {
                    mScrollLeft += child.getPosition().getWidth();
                }
            }
        }
    }

    public void syncScrollPosition(int scrollTop, int scrollLeft) {

        mLastScrollTop = scrollTop;
        mLastScrollLeft = scrollLeft;
        LynxEvent eventToJs = new LynxEvent(SCROLL_EVENT_TAG);
        // TODO: 17/8/2
        updateData(RenderObjectAttr.SCROLL_TOP, scrollTop);
        updateData(RenderObjectAttr.SCROLL_LEFT, scrollLeft);
        postEvent(SCROLL_EVENT_TAG, eventToJs);
    }

    public void smoothScrollTop(final int targetScrollTop) {
        if (mLinearLayoutManager.getOrientation() != LinearLayoutManager.VERTICAL)
            return;

        mView.post(new Runnable() {
            @Override
            public void run() {
                mView.smoothScrollBy(0, targetScrollTop - mScrollTop);
            }
        });
    }

    public void smoothScrollLeft(final int targetScrollLeft) {
        if (mLinearLayoutManager.getOrientation() != LinearLayoutManager.HORIZONTAL)
            return;

        mView.post(new Runnable() {
            @Override
            public void run() {
                mView.smoothScrollBy(targetScrollLeft - mScrollLeft, 0);
            }
        });
    }

    public void scrollTop(final int scrollTop) {
        if (mLinearLayoutManager.getOrientation() != LinearLayoutManager.VERTICAL)
            return;

        mView.post(new Runnable() {
            @Override
            public void run() {
                scrollInAxis(scrollTop);
            }
        });
    }

    public void scrollLeft(final int scrollLeft) {
        if (mLinearLayoutManager.getOrientation() != LinearLayoutManager.HORIZONTAL)
            return;

        mView.post(new Runnable() {
            @Override
            public void run() {
                scrollInAxis(scrollLeft);
            }
        });
    }

    private void scrollInAxis(int scrollDistance) {
        int totalSize = 0;
        int targetSection = -1;
        for (int i = 0; i < mAdapter.getItemCount(); i++) {
            if (mItemHelper.getNode(i) == null || mItemHelper.getNode(i).getPosition() == null) {
                return;
            }
            totalSize += mLinearLayoutManager.getOrientation() == LinearLayoutManager.HORIZONTAL ?
                    mItemHelper.getNode(i).getPosition().getWidth() :
                    mItemHelper.getNode(i).getPosition().getHeight();

            targetSection = i;
            if (scrollDistance - totalSize < 0) {
                break;
            }
        }
        int offsetStart = totalSize - scrollDistance;

        mLinearLayoutManager.scrollToPositionWithOffset(targetSection + 1, offsetStart);
    }

    private int getScrollTop() {
        return mScrollTop;
    }

    private int getScrollLeft() {
        return mScrollLeft;
    }

    public class RecyclerViewOnScrollListener extends RecyclerView.OnScrollListener {

        @Override
        public void onScrollStateChanged(RecyclerView recyclerView, int newState) {
        }

        @Override
        public void onScrolled(RecyclerView recyclerView, int dx, int dy) {

            // 允许传递滑动事件后开始传递给Js
            LynxUIRecyclerView proxy = LynxUIRecyclerView.this;
            obtainScrollInfo();
            int scrollTop = proxy.getScrollTop();
            int scrollLeft = proxy.getScrollLeft();
            proxy.syncScrollPosition(scrollTop, scrollLeft);
            mRecycler.onScrollItemChange(mLinearLayoutManager.findFirstVisibleItemPosition(),
                    mLinearLayoutManager.findLastVisibleItemPosition());
        }

    }

}
