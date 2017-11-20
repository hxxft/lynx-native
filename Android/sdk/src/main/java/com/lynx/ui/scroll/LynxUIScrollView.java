// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.ui.scroll;

import android.content.Context;
import android.view.View;
import android.view.ViewGroup;
import android.widget.LinearLayout;

import com.lynx.base.Constants;
import com.lynx.base.Size;
import com.lynx.base.Style;
import com.lynx.core.base.LynxEvent;
import com.lynx.core.impl.RenderObjectAttr;
import com.lynx.core.impl.RenderObjectImpl;
import com.lynx.ui.LynxUI;
import com.lynx.ui.LynxUIFactory;
import com.lynx.ui.LynxUIGroup;

public class LynxUIScrollView extends LynxUIGroup<AndroidScrollView> {
    private final static String EVENT_SCROLL = "scroll";
    private final static String EVENT_SCROLL_IDLE = "idle";
    private final static String EVENT_SCROLL_START = "scrollstart";
    private final static String EVENT_SCROLL_CANCEL = "scrollcancel";
    private final static String ATTR_START_THRESHOLD = "start-threshold";
    private final static String ATTR_END_THRESHOLD = "end-threshold";
    private final static String ATTR_SCROLL_WITH_ANIMATION = "scroll-with-animation";

    private InnerOnScrollListener mOnScrollListener;
    private int mStartThreshold = Constants.UNDEFINED;
    private int mEndeThreadshold = Constants.UNDEFINED;
    private boolean mScrollWithAnimation = true;

    public LynxUIScrollView(Context context, RenderObjectImpl impl) {
        super(context, impl);
    }

    @Override
    protected AndroidScrollView createView(Context context) {
        AndroidScrollView scrollView = new AndroidScrollView(context, this);
        mOnScrollListener = new InnerOnScrollListener();
        scrollView.setOnScrollListener(mOnScrollListener);
        return scrollView;
    }

    @Override
    public void updateStyle(Style style) {
        super.updateStyle(style);

        if (style == null) return;

        if (style.mFlexDirection == Style.CSSFLEX_DIRECTION_ROW) {
            mView.setOrientation(AndroidScrollView.HORIZONTAL);
        }
        if (style.mFlexDirection == Style.CSSFLEX_DIRECTION_COLUMN) {
            mView.setOrientation(AndroidScrollView.VERTICAL);
        }
    }

    @Override
    public void insertChild(RenderObjectImpl child, int index) {
        if (!child.hasUI()) {
            attachChildElement(child);
        }
        View impl = child.getUI().getView();
        if (impl.getParent() != null) {
            ((ViewGroup) impl.getParent()).removeView(impl);
        }
        mView.addView(impl, index);
    }

    protected void attachChildElement(RenderObjectImpl childElement) {
        LynxUI ui = LynxUIFactory.create(mView.getContext(), childElement);
        childElement.setUI(ui);
        for (int i = 0; i < childElement.getChildCount(); i++) {
            ui.insertChild(childElement.getChildAt(i), i);
        }
    }

    @Override
    public void removeChild(RenderObjectImpl child) {
        if ((child).hasUI()) {
            mView.removeView(child.getUI().getView());
        }
    }

    @Override
    public void setAttribute(String key, String value) {
        super.setAttribute(key, value);
        switch (key) {
            case ATTR_START_THRESHOLD:
                mStartThreshold = Integer.valueOf(value);
                break;
            case ATTR_END_THRESHOLD:
                mEndeThreadshold = Integer.valueOf(value);
                break;
            case ATTR_SCROLL_WITH_ANIMATION:
                mScrollWithAnimation = Boolean.valueOf(value);
                break;
            default:
                break;
        }
    }

    @Override
    public ViewGroup.LayoutParams generateLayoutParams(ViewGroup.LayoutParams childParams) {
        if (childParams instanceof LinearLayout.LayoutParams) {
            return childParams;
        }
        return new LinearLayout.LayoutParams(0, 0);
    }

    @Override
    public void setData(int attr, Object param) {
        super.setData(attr, param);
        if (attr == RenderObjectAttr.SCROLL_LEFT.value()) {
            mView.setScrollTo((Integer) param, 0, mScrollWithAnimation);
        } else if (attr == RenderObjectAttr.SCROLL_TOP.value()) {
            mView.setScrollTo(0, (Integer) param, mScrollWithAnimation);
        }
    }

    @Override
    public void setSize(Size size) {
        super.setSize(size);
        mView.setMeasuredSize(size);
    }

    // TODO: 17/8/2 和容器层级兼容问题，会引起内存泄露
//    @Override
//    public void onDestroy() {
//        super.onDestroy();
//        this.mViewImpl.setScrollContainer(false);
//
//        try {
//            Method e = View.class.getDeclaredMethod("dispatchDetachedFromWindow", new Class[0]);
//            e.setAccessible(true);
//            if(e != null) {
//                e.invoke(this.mViewImpl, new Object[0]);
//            }
//        } catch (InvocationTargetException var2) {
//            var2.printStackTrace();
//        } catch (IllegalAccessException var3) {
//            var3.printStackTrace();
//        } catch (NoSuchMethodException var4) {
//            var4.printStackTrace();
//        }
//    }

    @Override
    public void addEventListener(String event) {
        switch (event) {
            case EVENT_SCROLL:
                mOnScrollListener.setScrollEventEnabled(true);
                break;
            case EVENT_SCROLL_CANCEL:
                mOnScrollListener.setScrollCancelEventEnabled(true);
                break;
            case EVENT_SCROLL_START:
                mOnScrollListener.setScrollStartEventEnabled(true);
                break;
            case EVENT_SCROLL_IDLE:
                break;
            default:
                break;
        }
    }

    @Override
    public void removeEventListener(String event) {
        switch (event) {
            case EVENT_SCROLL:
                mOnScrollListener.setScrollEventEnabled(false);
                break;
            case EVENT_SCROLL_CANCEL:
                mOnScrollListener.setScrollCancelEventEnabled(false);
                break;
            case EVENT_SCROLL_START:
                mOnScrollListener.setScrollStartEventEnabled(false);
                break;
            case EVENT_SCROLL_IDLE:
                break;
            default:
                break;
        }
    }

    private class InnerOnScrollListener implements AndroidScrollView.OnScrollListener {
        private LynxEvent mScrollEventToJs;
        private LynxEvent mScrollStartEventToJs;
        private LynxEvent mScrollCancelEventToJs;
        private boolean mScrollEnabled;
        private boolean mScrollStartEnabled;
        private boolean mScrollCancelEnabled;

        InnerOnScrollListener() {
        }

        @Override
        public void onScrollStop() {

        }

        @Override
        public void onScrollChanged(int l, int t, int oldl, int oldt) {
            LynxUIScrollView.this.updateData(RenderObjectAttr.SCROLL_LEFT, l);
            LynxUIScrollView.this.updateData(RenderObjectAttr.SCROLL_TOP, t);
            LynxUIScrollView.this.dispatchCoordinatorScroll(t, l);
            if (!mScrollEnabled) return;
            mScrollEventToJs = new LynxEvent(EVENT_SCROLL);
            LynxUIScrollView.this.postEvent(EVENT_SCROLL, mScrollEventToJs);
        }

        @Override
        public void onScrollStart() {
            if (!mScrollStartEnabled) return;
            mScrollStartEventToJs = new LynxEvent(EVENT_SCROLL_START);
            LynxUIScrollView.this.postEvent(EVENT_SCROLL_START, mScrollStartEventToJs);
        }

        @Override
        public void onScrollCancel() {
            if (!mScrollCancelEnabled) return;
            mScrollCancelEventToJs = new LynxEvent(EVENT_SCROLL_CANCEL);
            LynxUIScrollView.this.postEvent(EVENT_SCROLL_CANCEL, mScrollCancelEventToJs);
        }

        void setScrollEventEnabled(boolean enabled) {
            mScrollEnabled = enabled;
        }

        void setScrollStartEventEnabled(boolean enabled) {
            mScrollStartEnabled = enabled;
        }

        void setScrollCancelEventEnabled(boolean enabled) {
            mScrollCancelEnabled = enabled;
        }

    }
}
