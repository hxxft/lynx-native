package com.lynx.gtest;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.util.Log;

public class GTestDriverReceiver extends BroadcastReceiver {

    private static final String LOG_TAG = "GTestDriverReceiver";

    @Override
    public void onReceive(Context context, Intent intent) {
        String gtestCmd = intent.getStringExtra("cmd");
        boolean onMainThread = intent.getBooleanExtra("mainlooper", false);
        if (onMainThread) {
            Log.i(LOG_TAG, "onReceiver: Scheduling tests with:" + gtestCmd);
            GTestDriver.runGTests(gtestCmd);
        } else {
            Log.i(LOG_TAG, "onReceiver: Executing test with" + gtestCmd);
            GTestDriver.scheduleGTests(gtestCmd);
        }
    }
}
