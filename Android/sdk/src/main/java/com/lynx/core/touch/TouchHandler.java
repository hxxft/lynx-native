// Copyright 2017 The Lynx Authors. All rights reserved.

package com.lynx.core.touch;

import android.content.Context;
import android.view.GestureDetector;
import android.view.MotionEvent;

public class TouchHandler extends GestureDetector.SimpleOnGestureListener {

    private GestureDetector mGestureDetector;

    private MotionEventType mMotionType = MotionEventType.NOTHING;

    public TouchHandler(Context context) {
        mGestureDetector = new GestureDetector(context, this);
    }

    public TouchEventInfo handleMotionEvent(MotionEvent motionEvent) {
        mGestureDetector.onTouchEvent(motionEvent);

        TouchEventType touchEventType = TouchEventType.NOTHING;
        int action = (motionEvent.getAction() & MotionEvent.ACTION_MASK);
        switch (action) {
            case MotionEvent.ACTION_DOWN:
                touchEventType = TouchEventType.START;
                break;
            case MotionEvent.ACTION_UP:
                touchEventType = TouchEventType.END;
                break;
            case MotionEvent.ACTION_POINTER_DOWN:
                touchEventType = TouchEventType.START;
                break;
            case MotionEvent.ACTION_POINTER_UP:
                touchEventType = TouchEventType.END;
                break;
            case MotionEvent.ACTION_MOVE:
                touchEventType = TouchEventType.MOVE;
                break;
            case MotionEvent.ACTION_CANCEL:
                touchEventType = TouchEventType.CANCEL;
                break;
            default:
                break;
        }

        TouchEventInfo info = new TouchEventInfo(motionEvent.getX(), motionEvent.getY(),
                motionEvent.getEventTime(), touchEventType, mMotionType);

        return info;
    }

    public void reset() {
        mMotionType = MotionEventType.NOTHING;
    }

    @Override
    public boolean onSingleTapUp(MotionEvent e) {
        mMotionType = MotionEventType.CLICK;
        return super.onSingleTapUp(e);
    }
}
