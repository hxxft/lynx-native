// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.ui;

import android.content.Context;

import com.lynx.core.impl.RenderObjectImpl;
import com.lynx.modules.ext.ModuleManager;
import com.lynx.ui.button.LynxUIButton;
import com.lynx.ui.canvas.LynxUICanvas;
import com.lynx.ui.checkbox.LynxUICheckBox;
import com.lynx.ui.image.LynxUIImage;
import com.lynx.ui.input.LynxUIInput;
import com.lynx.ui.label.LynxUILabel;
import com.lynx.ui.radio.LynxUIRadio;
import com.lynx.ui.radio.LynxUIRadioGroup;
import com.lynx.ui.recycler.LynxUIRecyclerView;
import com.lynx.ui.scroll.LynxUIScrollView;
import com.lynx.ui.slider.LynxUISlider;
import com.lynx.ui.swiper.LynxUISwiper;
import com.lynx.ui.textarea.LynxUITextArea;
import com.lynx.ui.uiswitch.LynxUISwitch;
import com.lynx.ui.view.LynxUIView;

public class LynxUIFactory {

    public final static int UI_TYPE_NONE = -1;
    public final static int UI_TYPE_BODY = 0;
    public final static int UI_TYPE_VIEW = 1;
    public final static int UI_TYPE_LABEL = 2;
    public final static int UI_TYPE_LISTVIEW = 3;
    public final static int UI_TYPE_CELLVIEW= 4;
    public final static int UI_TYPE_LIST_SHADOW = 5;
    public final static int UI_TYPE_IMAGE = 6;
    public final static int UI_TYPE_SCROLLVIEW = 7;
    public final static int UI_TYPE_INPUT = 8;
    public final static int UI_TYPE_SWIPER = 9;
    public final static int UI_TYPE_LAYOUT_VIEW = 10;
    public final static int UI_TYPE_TEXT_NODE = 11;
    public final static int UI_TYPE_TEXT_AREA = 12;
    public final static int UI_TYPE_SWITCH = 13;
    public final static int UI_TYPE_SLIDER = 14;
    public final static int UI_TYPE_BUTTON = 15;
    public final static int UI_TYPE_RADIO = 16;
    public final static int UI_TYPE_RADIO_GROUP = 17;
    public final static int UI_TYPE_CHECKBOX = 18;
    public final static int UI_TYPE_CANVAS = 19;

    public static LynxUI create(Context context, RenderObjectImpl impl) {
        LynxUI ui = null;

        switch (impl.getRenderObjectType()) {
            case UI_TYPE_VIEW:
            case UI_TYPE_CELLVIEW:
            case UI_TYPE_LIST_SHADOW:
                ui = new LynxUIView(context, impl);
                break;
            case UI_TYPE_LABEL:
                ui = new LynxUILabel(context, impl);
                break;
            case UI_TYPE_IMAGE:
                ui = new LynxUIImage(context, impl);
                break;
            case UI_TYPE_LISTVIEW:
                ui = new LynxUIRecyclerView(context, impl);
                break;
            case UI_TYPE_SCROLLVIEW:
                ui = new LynxUIScrollView(context, impl);
                break;
            case UI_TYPE_INPUT:
                ui = new LynxUIInput(context, impl);
                break;
            case UI_TYPE_SWIPER:
                ui = new LynxUISwiper(context, impl);
                break;
            case UI_TYPE_TEXT_AREA:
                ui = new LynxUITextArea(context, impl);
                break;
            case UI_TYPE_SWITCH:
                ui = new LynxUISwitch(context, impl);
                break;
            case UI_TYPE_SLIDER:
                ui = new LynxUISlider(context, impl);
                break;
            case UI_TYPE_BUTTON:
                ui = new LynxUIButton(context, impl);
                break;
            case UI_TYPE_RADIO:
                ui = new LynxUIRadio(context, impl);
                break;
            case UI_TYPE_RADIO_GROUP:
                ui = new LynxUIRadioGroup(context, impl);
                break;
            case UI_TYPE_CHECKBOX:
                ui = new LynxUICheckBox(context, impl);
                break;
            case UI_TYPE_CANVAS:
                ui = new LynxUICanvas(context, impl);
                break;
            default:
                ui = ModuleManager.createExtLynxUI(impl.getRenderObjectType(),context,impl);
                break;
        }
        return ui;
    }

}
