// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.ui.swiper;

import android.content.Context;

import com.lynx.ui.LynxUI;

public class AndroidSwiper extends SwiperView {

    private LynxUI mUI;

    public AndroidSwiper(Context Context, LynxUI ui) {
        super(Context);
        mUI = ui;
    }

    @Override
    public SliderViewPagerAdapter createSliderViewPagerAdapter(Context context) {
        return new SliderViewPagerAdapter(context);
    }

    @Override
    public LooperController createLooperController() {
        return new LooperController(this);
    }

}
