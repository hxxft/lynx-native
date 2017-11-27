// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.ui.coordinator;

import android.view.MotionEvent;

import com.lynx.ui.LynxUI;
import com.lynx.utils.PixelUtil;

/**
 * For {@link CoordinatorResponder} to do treatment
 */
public class Treatment {

    public final static String COMMAND_INIT = "init";
    public final static String COMMAND_UPDATE_PROPERTIES = "onPropertiesUpdated";
    public final static String ATTR_COORDINATOR_COMMAND = "coordinator-command";

    private CoordinatorCommands mCommand;
    private boolean mInit;
    private LynxUI mUI;
    private CoordinatorExecutor mCResultExecutor;

    public Treatment(LynxUI ui) {
        mUI = ui;
        mCResultExecutor = new CoordinatorExecutor(ui);
    }

    public void addCoordinatorCommand(String content) {
        mCommand = new CoordinatorCommands(content);
    }

    public void init(CommandExecutor executor) {
        if (!mInit) {
            mInit = true;
            CoordinatorResult result = executor.executeCommand(COMMAND_INIT, mUI.coordinatorTag());
            mCResultExecutor.execute(result);
        }
    }

    public void onPropertiesUpdated(CommandExecutor executor) {
        CoordinatorResult result = executor.executeCommand(COMMAND_UPDATE_PROPERTIES,
                mUI.coordinatorTag());
        mCResultExecutor.execute(result);
    }

    public void onNestedAction(String type, CommandExecutor executor, Object... params) {
        if (mCommand == null) return;
        switch (type) {
            case CoordinatorTypes.SCROLL:
                onNestedScroll(executor, (int) params[0], (int) params[1]);
                break;
            case CoordinatorTypes.TOUCH:
                onNestedTouch(executor, (MotionEvent) params[0]);
                break;
            default: break;
        }
    }

    private void onNestedScroll(CommandExecutor executor, int scrollTop, int scrollLeft) {
        String tag = mUI.coordinatorTag();
        String command = mCommand.getCommand(CoordinatorTypes.SCROLL);
        if (command == null) return;
        CoordinatorResult result = executor.executeCommand(command,
                tag,
                PixelUtil.pxToLynxNumber(scrollTop),
                PixelUtil.pxToLynxNumber(scrollLeft));
        mCResultExecutor.execute(result);
    }

    private void onNestedTouch(CommandExecutor executor, MotionEvent event) {
        float touchX = event.getY();
        float touchY = event.getX();
        String tag = mUI.coordinatorTag();
        String command = mCommand.getCommand(CoordinatorTypes.TOUCH);
        if (command == null) return;
        CoordinatorResult result = executor.executeCommand(command,
                tag,
                PixelUtil.pxToLynxNumber(touchX),
                PixelUtil.pxToLynxNumber(touchY));
        mCResultExecutor.execute(result);
    }

}
