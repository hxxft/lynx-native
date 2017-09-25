// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.ui.uiswitch;

import android.content.Context;
import android.support.v7.widget.SwitchCompat;
import android.widget.CompoundButton;

import com.lynx.core.base.LynxEvent;
import com.lynx.core.base.LynxObject;
import com.lynx.core.impl.RenderObjectImpl;
import com.lynx.ui.LynxUI;

public class LynxUISwitch extends LynxUI<SwitchCompat> {
    private final static String EVENT_CHANGE = "change";
    private final static String ATTR_CHECKED = "checked";

    public LynxUISwitch(Context context, RenderObjectImpl impl) {
        super(context, impl);
    }

    @Override
    protected SwitchCompat createView(Context context) {
        return new SwitchCompat(context);
    }

    @Override
    public void setAttribute(String key, String value) {
        super.setAttribute(key, value);
        switch (key) {
            case ATTR_CHECKED:
                setChecked(mRenderObjectImpl);
                break;
            default:
                break;
        }
    }

    protected void setChecked(RenderObjectImpl impl) {
        boolean checked = Boolean.valueOf(impl.getAttribute(ATTR_CHECKED));
        mView.setChecked(checked);
    }

    @Override
    public void addEventListener(String event) {
        super.addEventListener(event);
        switch (event) {
            case EVENT_CHANGE:
                mView.setOnCheckedChangeListener(new InnerOnCheckedChangedListener());
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

    private class InnerOnCheckedChangedListener
            implements CompoundButton.OnCheckedChangeListener {

        @Override
        public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
            LynxEvent event = new LynxEvent(EVENT_CHANGE);
            LynxObject detail = new LynxObject();
            detail.setProperty("checked", isChecked);
            event.setProperty("detail", detail);
            postEvent(EVENT_CHANGE, event);
        }
    }
}
