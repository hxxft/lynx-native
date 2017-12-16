// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.ui.coordinator;

import android.view.MotionEvent;

import com.lynx.utils.PixelUtil;

/**
 * For {@link CrdResponder} to do treatment
 */
public class CrdTreatment {

    public final static String COMMAND_INIT = "init";
    public final static String COMMAND_UPDATE_PROPERTIES = "onPropertiesUpdated";
    public final static String ATTR_COORDINATOR_COMMAND = "coordinator-command";

    private CrdCommands mCommand;
    private boolean mInit;
    private CrdResponder mResponder;
    private CrdActionExecutor mCResultExecutor;

    public CrdTreatment(CrdResponder responder, CrdActionExecutor executor) {
        mResponder = responder;
        mCResultExecutor = executor;
    }

    public void addCoordinatorCommand(String content) {
        mCommand = new CrdCommands(content);
    }

    public void init(CrdCommandExecutor executor) {
        if (!mInit) {
            mInit = true;
            CrdResult result = executor.executeCommand(COMMAND_INIT, mResponder.coordinatorTag());
            mCResultExecutor.execute(result);
        }
    }

    public void reset() {
        mInit = false;
    }

    public void onPropertiesUpdated(CrdCommandExecutor executor) {
        CrdResult result = executor.executeCommand(COMMAND_UPDATE_PROPERTIES,
                mResponder.coordinatorTag());
        mCResultExecutor.execute(result);
    }

    public void onNestedAction(String type, CrdCommandExecutor executor, Object... params) {
        if (mCommand == null) return;
        switch (type) {
            case CrdTypes.SCROLL:
                onNestedScroll(executor, (int) params[0], (int) params[1]);
                break;
            case CrdTypes.TOUCH:
                onNestedTouch(executor, (MotionEvent) params[0]);
                break;
            default: break;
        }
    }

    private void onNestedScroll(CrdCommandExecutor executor, int scrollTop, int scrollLeft) {
        String tag = mResponder.coordinatorTag();
        String command = mCommand.getCommand(CrdTypes.SCROLL);
        if (command == null) return;
        CrdResult result = executor.executeCommand(command,
                tag,
                PixelUtil.pxToLynxNumber(scrollTop),
                PixelUtil.pxToLynxNumber(scrollLeft));
        mCResultExecutor.execute(result);
    }

    private void onNestedTouch(CrdCommandExecutor executor, MotionEvent event) {
        float touchX = event.getY();
        float touchY = event.getX();
        String tag = mResponder.coordinatorTag();
        String command = mCommand.getCommand(CrdTypes.TOUCH);
        if (command == null) return;
        CrdResult result = executor.executeCommand(command,
                tag,
                PixelUtil.pxToLynxNumber(touchX),
                PixelUtil.pxToLynxNumber(touchY));
        mCResultExecutor.execute(result);
    }

}
