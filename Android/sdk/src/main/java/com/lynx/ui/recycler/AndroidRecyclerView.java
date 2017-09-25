// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.ui.recycler;

import android.content.Context;
import android.support.v7.widget.RecyclerView;


public class AndroidRecyclerView extends RecyclerView {
    private LynxUIRecyclerView mUI;

    public AndroidRecyclerView(Context context, LynxUIRecyclerView ui) {
        super(context);
        mUI = ui;
    }

    @Override
    protected void onLayout(boolean changed, int l, int t, int r, int b) {
        super.onLayout(changed, l, t, r, b);
    }

}
