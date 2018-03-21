// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.ui;

import android.content.Context;
import android.graphics.Matrix;
import android.view.View;
import android.view.ViewGroup;

import com.lynx.core.impl.RenderObjectImpl;
import com.lynx.core.touch.TouchAxis;
import com.lynx.core.touch.TouchTarget;
import com.lynx.ui.anim.matrix.MatrixAnimation;

public abstract class LynxUIGroup<T extends ViewGroup>
        extends LynxUI<T> {

    private static final Matrix mInverseMatrix = new Matrix();
    private static final float[] mMatrixTransformCoords = new float[2];

    public LynxUIGroup(Context context) {
        super(context);
    }

    public LynxUIGroup(Context context, RenderObjectImpl impl) {
        super(context, impl);
    }

    @Override
    public abstract void insertChild(RenderObjectImpl child, int index);

    @Override
    public abstract void removeChild(RenderObjectImpl child);

    /**
     * Generating a suitable LayoutParams for child.
     *
     * @param childParams childParams to check if child ui need
     *                    a new LayoutParams
     * @return LayoutParams, will be set into Android View if it
     * is not the same as childParams
     */
    public abstract ViewGroup.LayoutParams generateLayoutParams(
            ViewGroup.LayoutParams childParams);

    @Override
    public void measure() {
        measureChildren();
        super.measure();
    }

    public void measureChildren() {
        for (int i = 0; i < mRenderObjectImpl.getChildCount(); ++i) {
            RenderObjectImpl child = mRenderObjectImpl.getChildAt(i);
            if (child.hasUI()) {
                child.getUI().measure();
            }
        }
    }

    @Override
    public void layout() {
        super.layout();
        layoutChildren();
    }

    public void layoutChildren() {
        for (int i = 0; i < mRenderObjectImpl.getChildCount(); ++i) {
            RenderObjectImpl child = mRenderObjectImpl.getChildAt(i);
            if (child.hasUI()) {
                child.getUI().layout();
            }
        }
    }

    @Override
    public TouchTarget hitTest(TouchAxis axis) {
        TouchTarget handled = null;
        ViewGroup viewGroup = getHitTestView();

        for (int i = viewGroup.getChildCount() - 1; i >= 0; --i) {
            View childView = viewGroup.getChildAt(i);

            float offsetX = viewGroup.getScrollX() - childView.getLeft();
            float offsetY = viewGroup.getScrollY() - childView.getTop();

            axis.offset(offsetX, offsetY);

            if (checkIfHitInView(childView, axis)) {
                TouchTarget temp = getMatchTouchTarget(childView);
                if (temp != null) {
                    handled = temp.hitTest(axis);
                }
            }

            axis.offset(-offsetX, -offsetY);

            if (handled != null) return handled;
        }

        return super.hitTest(axis);
    }

    protected ViewGroup getHitTestView() {
        return mView;
    }

    protected TouchTarget getMatchTouchTarget(View view) {
        for (int j = 0; j < mRenderObjectImpl.getChildCount(); j++) {
            RenderObjectImpl childImpl = mRenderObjectImpl.getChildAt(j);
            if (childImpl.getUI() != null && childImpl.getUI().getView() == view) {
                return childImpl.getUI();
            }
        }
        return null;
    }

    protected boolean checkIfHitInView(View child, TouchAxis touchAxis) {

        float localX = touchAxis.x;
        float localY = touchAxis.y;

        Matrix matrix = null;
        if (child.getAnimation() != null
                && child.getAnimation() instanceof MatrixAnimation) {
            matrix = ((MatrixAnimation) child.getAnimation()).getMatrix();
        }

        if (matrix == null) {
            matrix = child.getMatrix();
        }

        if (!matrix.isIdentity()) {
            float[] localXY = mMatrixTransformCoords;
            localXY[0] = localX;
            localXY[1] = localY;
            Matrix inverseMatrix = mInverseMatrix;
            matrix.invert(inverseMatrix);
            inverseMatrix.mapPoints(localXY);
            localX = localXY[0];
            localY = localXY[1];
        }

        if ((localX >= 0 && localX < (child.getRight() - child.getLeft()))
                && (localY >= 0 && localY < (child.getBottom() - child.getTop()))) {
            touchAxis.setXY(localX, localY);
            return true;
        }

        return false;
    }

}
