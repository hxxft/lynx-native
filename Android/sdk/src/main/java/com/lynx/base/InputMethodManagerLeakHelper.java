package com.lynx.base;

import android.content.Context;
import android.view.inputmethod.InputMethodManager;

import java.lang.reflect.Field;


public class InputMethodManagerLeakHelper {
    public static void resolve(Context context) {
        if (context == null) {
            return;
        }
        try {
            InputMethodManager imm = (InputMethodManager) context.getSystemService(Context.INPUT_METHOD_SERVICE);
            if (imm == null) {
                return;
            }
            Object obj_get = null;
            Field f_mCurRootView = imm.getClass().getDeclaredField("mCurRootView");
            Field f_mServedView = imm.getClass().getDeclaredField("mServedView");
            Field f_mNextServedView = imm.getClass().getDeclaredField("mNextServedView");
            Field f_mLastSrvView = imm.getClass().getDeclaredField("mLastSrvView");
            if (f_mCurRootView.isAccessible() == false) {
                f_mCurRootView.setAccessible(true);
            }
            obj_get = f_mCurRootView.get(imm);
            if (obj_get != null) { // 不为null则置为空
                f_mCurRootView.set(imm, null);
            }
            if (f_mServedView.isAccessible() == false) {
                f_mServedView.setAccessible(true);
            }
            obj_get = f_mServedView.get(imm);
            if (obj_get != null) { // 不为null则置为空
                f_mServedView.set(imm, null);
            }
            if (f_mNextServedView.isAccessible() == false) {
                f_mNextServedView.setAccessible(true);
            }
            obj_get = f_mNextServedView.get(imm);
            if (obj_get != null) { // 不为null则置为空
                f_mNextServedView.set(imm, null);
            }

            if (f_mLastSrvView.isAccessible() == false) {
                f_mLastSrvView.setAccessible(true);
            }
            obj_get = f_mLastSrvView.get(imm);
            if (obj_get != null) { // 不为null则置为空
                f_mLastSrvView.set(imm, null);
            }
        } catch (Throwable t) {
            t.printStackTrace();
        }
    }
}
