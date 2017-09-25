// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.ui.input;

import android.content.Context;
import android.support.v7.widget.AppCompatEditText;
import android.view.inputmethod.InputMethodManager;

public class AndroidInput extends AppCompatEditText {

    private boolean mIsAutoFocus = false;
    private boolean mIsAttachedToWindow = false;

    public AndroidInput(Context context) {
        super(context);
    }

    public void setAutoFocus(boolean autoFocus) {
        mIsAutoFocus = autoFocus;
        if (mIsAttachedToWindow && mIsAutoFocus) {
            requestSoftInput();
        }
    }

    @Override
    protected void onAttachedToWindow() {
        super.onAttachedToWindow();
        mIsAttachedToWindow = true;
        if (mIsAutoFocus) {
            requestSoftInput();
        }
    }

    @Override
    protected void onDetachedFromWindow() {
        super.onDetachedFromWindow();
        mIsAttachedToWindow = false;
    }

    public void requestSoftInput() {
        this.requestFocus();
        InputMethodManager imm = (InputMethodManager) getContext()
                .getSystemService(Context.INPUT_METHOD_SERVICE);
        imm.showSoftInput(this, InputMethodManager.SHOW_IMPLICIT);
    }

}
