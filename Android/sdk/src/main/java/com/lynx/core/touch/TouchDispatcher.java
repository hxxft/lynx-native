// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.core.touch;

import android.content.Context;
import android.support.annotation.NonNull;
import android.view.MotionEvent;

import com.lynx.core.touch.gesture.GestureDispatcher;

import java.util.List;
import java.util.Stack;

public class TouchDispatcher {

    private Delegate mDelegate;
    private TouchTarget mRootTarget;
    private TouchHandler mTouchHandler;
    private Stack<TouchTarget> mTargetStack;
    private GestureDispatcher mGestureDispatcher;

    public TouchDispatcher(Context context, TouchTarget root) {
        mRootTarget = root;
        mTargetStack = new Stack<>();
        mTouchHandler = new TouchHandler();
        mDelegate = new TouchDispatcherDelegate();
        mGestureDispatcher = new GestureDispatcher(context);
    }

    public boolean dispatchEvent(MotionEvent event) {

        TouchEventInfo touchEventInfo = mTouchHandler.handleMotionEvent(event);

        if (event.getAction() == MotionEvent.ACTION_DOWN && mTargetStack.size() == 0) {
            hitTest(new TouchAxis(touchEventInfo.getTouchAxis()));
        }

        if (!mTargetStack.empty()) {
            touchEventInfo.setTarget(mTargetStack.lastElement());
        }

        // Capture
        captureEvent(touchEventInfo);

        if (!touchEventInfo.getCancelBubble()) {
            // Bubble
            bubbleEvent(touchEventInfo);
        }

        // Handle gesture event after finish handling touch event
        mGestureDispatcher.handle(touchEventInfo);

        if (event.getAction() == MotionEvent.ACTION_UP ||
                event.getAction() == MotionEvent.ACTION_CANCEL) {
            reset();
        }

        return touchEventInfo.getPreventDefault();
    }

    private void hitTest(TouchAxis touchAxis) {
        TouchTarget finalTarget = mRootTarget.hitTest(touchAxis);
        if (finalTarget != null) {
            List<TouchTarget> list = mDelegate.findPathByTarget(finalTarget);
            for (TouchTarget target : list) {
                mTargetStack.push(target);
                mGestureDispatcher.addWatcher(target);
            }
        }
    }

    private void captureEvent(TouchEventInfo touchEvent) {

        for (int i = 0; i < mTargetStack.size(); i++) {
            TouchTarget touchTarget = mTargetStack.get(i);

            touchEvent.setCurTarget(touchTarget);
            touchTarget.onCapturingTouchEvent(touchEvent);

            if (touchEvent.getCancelBubble()) {
                break;
            }
        }

    }

    private void bubbleEvent(TouchEventInfo touchEvent) {
        if (mTargetStack.empty()) return;
        touchEvent.setTarget(mTargetStack.lastElement());
        TouchTarget currentTarget;
        for (int i = mTargetStack.size() - 1; i >= 0; --i) {
            currentTarget = mTargetStack.get(i);

            // Handle self touch event
            handleTouch(currentTarget, touchEvent);

            // If bubble is false, cancel bubble
            if (touchEvent.getCancelBubble()) {
                break;
            }
        }
    }

    private void handleTouch(TouchTarget target, TouchEventInfo touchEvent) {
        touchEvent.setCurTarget(target);
        target.performTouch(touchEvent);
    }

    private void reset() {
        mTargetStack.clear();
        mTouchHandler.reset();
        mGestureDispatcher.reset();
    }

    interface Delegate {
        List<TouchTarget> findPathByTarget(@NonNull TouchTarget finalTarget);
    }
}
