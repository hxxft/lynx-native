// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.core.touch;

import android.content.Context;
import android.graphics.Matrix;
import android.util.SparseArray;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;

import com.lynx.core.touch.gesture.GestureHandler;
import com.lynx.ui.LynxUI;
import com.lynx.ui.LynxUIGroup;

import java.util.Stack;

public class TouchDispatcher {
    private GestureHandler mGestureHandler;
    private SparseArray<Stack<TouchTarget>> mTargetStackList;
    private Stack<TouchTarget> mCurTargetStack;
    private LynxUI mRootLynxUI;
    private float[] mMatrixTransformPTS = new float[2];
    private Matrix mInverseMatrix = new Matrix();

    public TouchDispatcher(Context context, LynxUI root) {
        mRootLynxUI = root;
        mTargetStackList = new SparseArray<>();
        mGestureHandler = new GestureHandler(context);
    }

    public void dispatchMotionEvent(MotionEvent event) {
        mGestureHandler.handle(event);
        dispatchTouchEvent(new TouchEventInfo(event));
    }

    private void dispatchTouchEvent(TouchEventInfo info) {
        // First assign stack
        assignStack(info);

        // Second capture only once when the touch starts
        if (info.getType().equals(TouchEventInfo.START)) {
            captureEvent(new TouchAxis(info.getAxis()), info);
        }

        // Third bubble every time
        bubbleEvent(info);

        // Last reset stack
        if (info.getType().equals(TouchEventInfo.END) ||
                info.getType().equals(TouchEventInfo.CANCEL)) {
            reset();
        }
    }

    private void assignStack(TouchEventInfo info) {

        Stack<TouchTarget> targetStack = mTargetStackList.get(info.getPointerID());

        if (targetStack == null) {
            targetStack = new Stack<>();
            mTargetStackList.put(info.getPointerID(), targetStack);
        }

        mCurTargetStack = targetStack;
    }

    private void captureEvent(TouchAxis axis, TouchEventInfo info) {
        LynxUI target = mRootLynxUI;
        View responder = target.getView();

        while (responder != null && target != null) {

            if (mCurTargetStack.size() == 0 || target != mCurTargetStack.get(0)) {
                mCurTargetStack.push(target);
                mGestureHandler.addGestureWatcher(target);

                // Throw capturing event, maybe some event listener want to response.
                target.onCapturingTouchEvent(info);
            }

            responder = responder instanceof ViewGroup ?
                    findNextResponder((ViewGroup) responder, axis) : null;

            if (responder != null) {
                target = findNextTarget((LynxUIGroup) target, responder);
            }

        }
    }

    // This method find next target in touch area without considering fixed or z-index
    private View findNextResponder(ViewGroup responder, TouchAxis touchAxis) {
        View nextResponder = null;
        // If node has children, dispatch touch event.
        for (int i = responder.getChildCount() - 1; i >= 0; --i) {
            View childResponder = responder.getChildAt(i);

            // Only solve active view
            if (childResponder.getVisibility() != View.VISIBLE
                    || !responder.isEnabled()) {
                continue;
            }

            // Check if touch in the region of the node.
            if (checkIfTouchInside(responder, childResponder, touchAxis)) {
                nextResponder = childResponder;
                break;
            }
        }

        return nextResponder;
    }

    private boolean checkIfTouchInside(View responder, View childResponder, TouchAxis touchAxis) {

        float curX = touchAxis.x + responder.getScrollX() - childResponder.getLeft();
        float curY = touchAxis.y + responder.getScrollY() - childResponder.getTop();

        Matrix matrix = childResponder.getMatrix();
        if (!matrix.isIdentity()) {
            mMatrixTransformPTS[0] = curX;
            mMatrixTransformPTS[1] = curY;
            matrix.invert(mInverseMatrix);
            mInverseMatrix.mapPoints(mMatrixTransformPTS);
            curX = mMatrixTransformPTS[0];
            curY = mMatrixTransformPTS[1];
        }

        if ((curX >= 0 && curX < (childResponder.getRight() - childResponder.getLeft()))
                && (curY >= 0 && curY < (childResponder.getBottom() - childResponder.getTop()))) {
            touchAxis.setXY(curX, curY);
            return true;
        }

        return false;
    }

    private LynxUI findNextTarget(LynxUIGroup target, View nextResponder) {
        for (int i = 0; i < target.getRenderObjectImpl().getChildCount(); ++i) {
            LynxUI childUI = target.getRenderObjectImpl().getChildAt(i).getUI();
            if (childUI != null && childUI.getView() == nextResponder) {
                return childUI;
            }
        }
        return target;
    }

    private void bubbleEvent(TouchEventInfo info) {
        TouchTarget currentTarget;
        for (int i = mCurTargetStack.size() - 1; i >= 0; --i) {
            currentTarget = mCurTargetStack.get(i);

            // Handle self touch event
            handleTouch(currentTarget, info);

            // If target need to stop propagation, then prevent bubbling.
            if (currentTarget.isStopPropagation()) {
                break;
            }
        }
    }

    private void handleTouch(TouchTarget target, TouchEventInfo info) {
        target.performTouch(info);
    }

    private void reset() {
        mCurTargetStack.clear();
        mGestureHandler.reset();
    }
}
