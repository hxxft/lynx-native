// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.test;

import android.app.Activity;
import android.os.SystemClock;
import android.view.MotionEvent;
import android.view.View;

public class MultiTouchGenerator {
    private View mDispatcherView;
    private PointerInfo[] mPointerInfoArray = {null, null, null, null, null};
    private int mPointerCount = 0;
    private long mLastDownTime;

    public MultiTouchGenerator(View view) {
        mDispatcherView = view;
    }

    public MultiTouchGenerator(Activity activity) {
        mDispatcherView = activity.getWindow().getDecorView();
    }

    public int down(float x, float y) {
        // 1. Allocates pointerId
        int pointerId = -1;
        for (int i = 0; i < mPointerInfoArray.length; ++i) {
            if (mPointerInfoArray[i] == null) {
                pointerId = i;
                break;
            }
        }
        if (pointerId < 0) return pointerId;
        // 2. Allocates pointerIndex
        mPointerCount++;
        // Finds suitable index to insert
        int pointerIndex = 0;
        for (int i = pointerId; i >= 0; --i) {
            if (mPointerInfoArray[i] != null) {
                pointerIndex = mPointerInfoArray[i].pointerIndex + 1;
                break;
            }
        }
        // Increases the index of the rest of pointerId
        for (int i = pointerId; i < mPointerInfoArray.length; ++i) {
            if (mPointerInfoArray[i] != null) {
                mPointerInfoArray[i].pointerIndex++;
            }
        }

        // 3. Generates MotionEvent
        PointerInfo pointerInfo = new PointerInfo(pointerId, pointerIndex,
                SystemClock.uptimeMillis(), x, y);
        mPointerInfoArray[pointerId] = pointerInfo;
        MotionEvent.PointerProperties[] pointerProps = new MotionEvent.PointerProperties[mPointerCount];
        MotionEvent.PointerCoords[] pointerCoords = new MotionEvent.PointerCoords[mPointerCount];
        for (int i = 0; i < mPointerInfoArray.length; i++) {
            PointerInfo temp = mPointerInfoArray[i];
            if (temp != null) {
                pointerProps[temp.pointerIndex] = createPointerProps(temp.pointerId, 1);
                pointerCoords[temp.pointerIndex] = createPointerCoords(temp.x, temp.y);
            }
        }

        // 4. Dispatchs events
        if (mPointerCount > 1) {
            dispatchEvent(MotionEvent.ACTION_POINTER_DOWN | (pointerInfo.pointerIndex << MotionEvent.ACTION_POINTER_INDEX_SHIFT),
                    pointerInfo.downTime, pointerInfo.downTime, mPointerCount, pointerProps, pointerCoords);
        } else {
            dispatchEvent(MotionEvent.ACTION_DOWN, pointerInfo.downTime,
                    pointerInfo.downTime, mPointerCount, pointerProps, pointerCoords);
        }

        if (mPointerCount == 1) {
            mLastDownTime = pointerInfo.downTime;
        }

        return pointerId;
    }

    public void up(int pointerId) {
        PointerInfo pointerInfo = mPointerInfoArray[pointerId];
        MotionEvent.PointerProperties[] pointerProps = new MotionEvent.PointerProperties[mPointerCount];
        MotionEvent.PointerCoords[] pointerCoords = new MotionEvent.PointerCoords[mPointerCount];
        for (PointerInfo info : mPointerInfoArray) {
            if (info != null) {
                pointerProps[info.pointerIndex] = createPointerProps(info.pointerId, 1);
                pointerCoords[info.pointerIndex] = createPointerCoords(info.x, info.y);
            }
        }
        if (mPointerCount - 1 == 0) {
            dispatchEvent(MotionEvent.ACTION_UP, mLastDownTime, SystemClock.uptimeMillis(),
                    mPointerCount, pointerProps, pointerCoords);
        } else {
            dispatchEvent(MotionEvent.ACTION_POINTER_UP | (pointerInfo.pointerIndex << MotionEvent.ACTION_POINTER_INDEX_SHIFT),
                    mLastDownTime, SystemClock.uptimeMillis(), mPointerCount,
                    pointerProps, pointerCoords);
        }
        mPointerCount--;
        mPointerInfoArray[pointerId] = null;
    }

    public void up(MultiTouchMoveParam param) {
        PointerInfo pointerInfo = mPointerInfoArray[param.pointerId];
        pointerInfo.x = param.x;
        pointerInfo.y = param.y;

        MotionEvent.PointerProperties[] pointerProps = new MotionEvent.PointerProperties[mPointerCount];
        MotionEvent.PointerCoords[] pointerCoords = new MotionEvent.PointerCoords[mPointerCount];
        for (PointerInfo info : mPointerInfoArray) {
            pointerProps[info.pointerIndex] = createPointerProps(info.pointerId, 1);
            pointerCoords[info.pointerIndex] = createPointerCoords(info.x, info.y);
        }
        if (mPointerCount - 1 == 0) {
            dispatchEvent(MotionEvent.ACTION_UP, mLastDownTime, SystemClock.uptimeMillis(),
                    mPointerCount, pointerProps, pointerCoords);
        } else {
            dispatchEvent(MotionEvent.ACTION_POINTER_UP | (pointerInfo.pointerIndex << MotionEvent.ACTION_POINTER_INDEX_SHIFT),
                    mLastDownTime, SystemClock.uptimeMillis(), mPointerCount,
                    pointerProps, pointerCoords);
        }
        mPointerCount--;
    }

    public void move(MultiTouchMoveParam... params) {
        for (MultiTouchMoveParam param : params) {
            PointerInfo info = mPointerInfoArray[param.pointerId];
            info.x = param.x;
            info.y = param.y;
        }
        MotionEvent.PointerProperties[] pointerProps = new MotionEvent.PointerProperties[mPointerCount];
        MotionEvent.PointerCoords[] pointerCoords = new MotionEvent.PointerCoords[mPointerCount];
        for (PointerInfo info : mPointerInfoArray) {
            if (info != null) {
                pointerProps[info.pointerIndex] = createPointerProps(info.pointerId, 1);
                pointerCoords[info.pointerIndex] = createPointerCoords(info.x, info.y);
            }
        }
        dispatchEvent(MotionEvent.ACTION_MOVE, mLastDownTime, SystemClock.uptimeMillis(), mPointerCount,
                pointerProps, pointerCoords);
    }

    public void cancel() {
        MotionEvent.PointerProperties[] pointerProps = new MotionEvent.PointerProperties[mPointerCount];
        MotionEvent.PointerCoords[] pointerCoords = new MotionEvent.PointerCoords[mPointerCount];
        for (PointerInfo info : mPointerInfoArray) {
            if (info != null) {
                pointerProps[info.pointerIndex] = createPointerProps(info.pointerId, 1);
                pointerCoords[info.pointerIndex] = createPointerCoords(info.x, info.y);
            }
        }
        dispatchEvent(MotionEvent.ACTION_MOVE, mLastDownTime, SystemClock.uptimeMillis(), mPointerCount,
                pointerProps, pointerCoords);
    }

    private MotionEvent.PointerProperties createPointerProps(int id, int toolType) {
        MotionEvent.PointerProperties pointerProps = new MotionEvent.PointerProperties();
        pointerProps.id = id;
        pointerProps.toolType = toolType;
        return pointerProps;
    }

    private MotionEvent.PointerCoords createPointerCoords(float x, float y) {
        MotionEvent.PointerCoords pointerCoords = new MotionEvent.PointerCoords();
        pointerCoords.x = x;
        pointerCoords.y = y;
        return pointerCoords;
    }

    private void dispatchEvent(final int action, final long startTime, final long eventTime,
                               final int pointerCount,
                               final MotionEvent.PointerProperties[] pointerProps,
                               final MotionEvent.PointerCoords[] pointerCoords) {
        mDispatcherView.post(
                new Runnable() {
                    @Override
                    public void run() {
                        MotionEvent event =
                                MotionEvent.obtain(startTime, eventTime, action,
                                        pointerCount, pointerProps, pointerCoords,
                                        0, 0, 1.0f, 1.0f, 0, 0, 0, 0);
                        mDispatcherView.dispatchTouchEvent(event);
                        event.recycle();
                    }
                });
    }

    public static class MultiTouchMoveParam {
        int pointerId;
        float x;
        float y;

        public MultiTouchMoveParam(int pointerId, float x, float y) {
            this.pointerId = pointerId;
            this.x = x;
            this.y = y;
        }
    }

    private static class PointerInfo {
        int pointerId;
        int pointerIndex;
        long downTime;
        float x;
        float y;

        public PointerInfo(int pointerId, int pointerIndex, long downTime, float x, float y) {
            this.pointerId = pointerId;
            this.pointerIndex = pointerIndex;
            this.downTime = downTime;
            this.x = x;
            this.y = y;
        }

    }
}
