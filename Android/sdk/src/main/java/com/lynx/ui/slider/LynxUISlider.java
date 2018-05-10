// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.ui.slider;

import android.content.Context;
import android.support.v7.widget.AppCompatSeekBar;
import android.widget.SeekBar;

import com.lynx.core.base.LynxEvent;
import com.lynx.core.base.LynxMap;
import com.lynx.core.impl.RenderObjectImpl;
import com.lynx.ui.LynxUI;

public class LynxUISlider extends LynxUI<AppCompatSeekBar>{

    private final static String ATTR_MIN = "min";
    private final static String ATTR_MAX = "max";
    private final static String ATTR_STEP = "step";
    private final static String ATTR_VALUE = "value";
    private final static String ATTR_ACTIVE_LINE_COLOR = "active-line-color";
    private final static String ATTR_BACKGROUND_LINE_COLOR = "background-line-color";
    private final static String EVENT_CHANGE = "change";

    private final static int DEFAULT_MAX = 100;
    private final static int DEFAULT_MIN = 0;

    private int mSliderMax;
    private int mSliderMin;
    private int mMax;
    private int mMin;
    private int mStep;

    private boolean mIsChangeEventEnabled;

    private InnerOnSeekBarChangeListener mListener;

    public LynxUISlider(Context context, RenderObjectImpl impl) {
        super(context, impl);
    }

    @Override
    public void initialize() {
        super.initialize();
        mMax = DEFAULT_MAX;
        mMin = DEFAULT_MIN;
        mSliderMax = DEFAULT_MAX;
        mSliderMin = DEFAULT_MIN;
        mStep = 0;
        mIsChangeEventEnabled = false;
        mListener = new InnerOnSeekBarChangeListener();
    }

    @Override
    protected AppCompatSeekBar createView(Context context) {
        AppCompatSeekBar seekBar = new AppCompatSeekBar(context);
        seekBar.setOnSeekBarChangeListener(mListener);
        seekBar.setMax(mSliderMax);
        return seekBar;
    }

    @Override
    public void setAttribute(String key, String value) {
        super.setAttribute(key, value);
        switch (key) {
            case ATTR_MAX:
                setMax(Integer.valueOf(value));
                break;
            case ATTR_MIN:
                setMin(Integer.valueOf(value));
                break;
            case ATTR_STEP:
                setStep(Integer.valueOf(value));
                break;
            case ATTR_VALUE:
                setProgress(Integer.valueOf(value));
                break;
            case ATTR_ACTIVE_LINE_COLOR:
                break;
            case ATTR_BACKGROUND_LINE_COLOR:
                break;
            default:
                break;
        }
    }

    protected void setMax(int max) {
        mMax = max;
        mSliderMax = mMax - mMin;
        mView.setMax(mSliderMax);
    }

    protected void setMin(int min) {
        mMin = min;
        mSliderMax = mMax - mMin;
        mView.setMax(mSliderMax);
    }

    protected void setStep(int step) {
        mStep = step * (mSliderMax - mSliderMin) / (mMax - mMin);
    }

    protected void setProgress(int progress) {
        progress = progress * (mSliderMax - mSliderMin) / (mMax - mMin) + mSliderMin;
        mView.setProgress(progress);
    }

    @Override
    public void addEventListener(String event) {
        super.addEventListener(event);
        switch (event) {
            case EVENT_CHANGE:
                mIsChangeEventEnabled = true;
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
                mIsChangeEventEnabled = false;
                break;
            default:
                break;
        }
    }

    private class InnerOnSeekBarChangeListener implements SeekBar.OnSeekBarChangeListener {

        @Override
        public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {

            if (mStep != 0) {
                progress = progress / mStep;
                progress = progress * mStep;
                mView.setProgress(progress);
            }

            if (mIsChangeEventEnabled) {
                LynxEvent event = new LynxEvent(EVENT_CHANGE);
                LynxMap detail = new LynxMap();
                detail.set("progress",
                        progress * (mMax - mMin) / (mSliderMax - mSliderMin) + mMin);
                event.set("detail", detail);
                postEvent(EVENT_CHANGE, event);
            }

        }

        @Override
        public void onStartTrackingTouch(SeekBar seekBar) {

        }

        @Override
        public void onStopTrackingTouch(SeekBar seekBar) {

        }
    }

}
