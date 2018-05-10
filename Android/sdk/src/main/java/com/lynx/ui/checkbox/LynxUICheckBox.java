// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.ui.checkbox;

import android.content.Context;
import android.support.v7.widget.AppCompatCheckBox;
import android.widget.CompoundButton;

import com.lynx.core.base.LynxEvent;
import com.lynx.core.base.LynxMap;
import com.lynx.core.impl.RenderObjectImpl;
import com.lynx.ui.LynxUI;


public class LynxUICheckBox extends LynxUI<AppCompatCheckBox> {
    private final static String EVENT_CHANGE = "change";

    public LynxUICheckBox(Context context, RenderObjectImpl impl) {
        super(context, impl);
    }

    @Override
    protected AppCompatCheckBox createView(Context context) {
        return new AppCompatCheckBox(context);
    }

    @Override
    public void setAttribute(String key, String value) {
        super.setAttribute(key, value);
        if (key.equals("checked")) {
            setChecked(Boolean.valueOf(value));
        }
    }

    @Override
    public void setText(String text) {
        mView.setText(text);
    }

    public void setChecked(boolean checked) {
        mView.setChecked(checked);
    }

    @Override
    public void addEventListener(String event) {
        super.addEventListener(event);
        switch (event) {
            case EVENT_CHANGE:
                mView.setOnCheckedChangeListener(new InnerOnCheckedChangeListener());
                break;
            default:
                break;
        }
    }

    @Override
    public void removeEventListener(String event) {
        super.removeEventListener(event);
        switch (event) {
            case EVENT_CHANGE:
                mView.setOnCheckedChangeListener(null);
                break;
            default:
                break;
        }
    }

    private class InnerOnCheckedChangeListener implements CompoundButton.OnCheckedChangeListener {

        @Override
        public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
            LynxEvent event = new LynxEvent(EVENT_CHANGE);
            LynxMap detail = new LynxMap();
            detail.set("checked", isChecked);
            event.set("detail", detail);
            postEvent(EVENT_CHANGE, event);
        }
    }
}
