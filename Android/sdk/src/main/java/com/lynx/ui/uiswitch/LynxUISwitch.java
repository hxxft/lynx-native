// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.ui.uiswitch;

import android.content.Context;
import android.graphics.Color;
import android.graphics.PorterDuff;
import android.support.v7.widget.SwitchCompat;
import android.text.TextUtils;
import android.widget.CompoundButton;

import com.lynx.core.base.LynxEvent;
import com.lynx.core.base.LynxMap;
import com.lynx.core.impl.RenderObjectImpl;
import com.lynx.ui.LynxUI;

public class LynxUISwitch extends LynxUI<SwitchCompat> {
    private final static String EVENT_CHANGE = "change";
    private final static String ATTR_CHECK = "check";
    private final static String ATTR_TRACK_COLOR = "trackcolor";
    private final static String ATTR_THUMB_COLOR = "thumbcolor";

    public LynxUISwitch(Context context, RenderObjectImpl impl) {
        super(context, impl);
    }

    @Override
    protected SwitchCompat createView(Context context) {
        SwitchCompat switchCompat = new SwitchCompat(context);
        switchCompat.setShowText(false);
        return switchCompat;
    }

    @Override
    public void setAttribute(String key, String value) {
        super.setAttribute(key, value);
        switch (key) {
            case ATTR_CHECK:
                setChecked(mRenderObjectImpl);
                break;
            case ATTR_THUMB_COLOR:
                setThumbColor(mRenderObjectImpl);
                break;
            case ATTR_TRACK_COLOR:
                setTrackColor(mRenderObjectImpl);
                break;
            default:
                break;
        }
    }


    protected  void setTrackColor(RenderObjectImpl impl){
        final String trackColor = impl.getAttribute(ATTR_TRACK_COLOR);
        if(!TextUtils.isEmpty(trackColor)){
            try {
                mView.getTrackDrawable()
                        .setColorFilter(Color.parseColor(trackColor),PorterDuff.Mode.MULTIPLY);
            }catch (Exception e){

            }
        }
    }

    protected void setThumbColor(RenderObjectImpl impl){
        final String thumbColor = impl.getAttribute(ATTR_THUMB_COLOR);
        if(!TextUtils.isEmpty(thumbColor)){
            try {
                mView.getThumbDrawable()
                        .setColorFilter(Color.parseColor(thumbColor),PorterDuff.Mode.MULTIPLY);
            }catch (Exception e){

            }
        }
    }

    protected void setChecked(RenderObjectImpl impl) {
        boolean checked = Boolean.valueOf(impl.getAttribute(ATTR_CHECK));
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
            LynxMap detail = new LynxMap();
            detail.set("checked", isChecked);
            event.set("detail", detail);
            postEvent(EVENT_CHANGE, event);
        }
    }
}
