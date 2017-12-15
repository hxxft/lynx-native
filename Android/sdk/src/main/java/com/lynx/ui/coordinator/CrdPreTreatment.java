// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.ui.coordinator;

import android.view.MotionEvent;

import com.lynx.utils.PixelUtil;

/**
 * For {@link CrdSponsor} to do pre treatment
 */
public class CrdPreTreatment {
    private final static String DISPATCH_SCROLL_COMMAND = "onDispatchScrollEvent";
    private final static String DISPATCH_TOUCH_COMMAND = "onDispatchTouchEvent";

    public CrdPreTreatment() {
    }

    public boolean dispatchAction(String type, CrdCommandExecutor executor, String tag,
                                  Object... params) {
        switch (type) {
            case CrdTypes.SCROLL:
                return dispatchScroll(executor,  tag,
                        (int) params[0], (int) params[1]);
            case CrdTypes.TOUCH:
                return dispatchTouch(executor,  tag,
                        (MotionEvent) params[0]);
            default: break;
        }
        return false;
    }

    private boolean dispatchScroll(CrdCommandExecutor executor, String tag,
                                   int scrollTop, int scrollLeft) {
        CrdResult result = executor.executeCommand(DISPATCH_SCROLL_COMMAND,
                tag,
                PixelUtil.pxToLynxNumber(scrollTop),
                PixelUtil.pxToLynxNumber(scrollLeft));
        return result != null && result.isConsumed();
    }

    private boolean dispatchTouch(CrdCommandExecutor executor, String tag, MotionEvent event) {
//        ACTION_DOWN             = 0;
//        ACTION_UP               = 1;
//        ACTION_MOVE             = 2;
//        ACTION_CANCEL           = 3;
        int type = event.getAction();
        float axisX = event.getX();
        float axisY = event.getY();
        CrdResult result = executor.executeCommand(DISPATCH_TOUCH_COMMAND,
                tag,
                type,
                PixelUtil.pxToLynxNumber(axisX),
                PixelUtil.pxToLynxNumber(axisY),
                event.getEventTime());
        return result != null && result.isConsumed();
    }

}
