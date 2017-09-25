// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.ui.recycler;

import android.support.v7.widget.RecyclerView;
import android.view.View;
import android.view.ViewGroup;

import com.lynx.ui.LynxUI;

public class RecyclerViewAdapter extends RecyclerView.Adapter<RecyclerViewAdapter.VH> {

    private RecyclableItemHelper mItemHelper;
    private Recycler mRecycler;

    public RecyclerViewAdapter(Recycler recycler,
                               RecyclableItemHelper itemHelper) {
        mRecycler = recycler;
        mItemHelper = itemHelper;
    }

    @Override
    public VH onCreateViewHolder(ViewGroup parent, int viewType) {
        LynxUI ui = mRecycler.getRecyclerUI(viewType);
        // Trigger ui children measure and layout
        ui.measure();
        ui.layout();
        VH holder = new VH(ui.getView());
        holder.setIsRecyclable(false);
        return holder;
    }

    @Override
    public void onBindViewHolder(VH holder, int position) {
    }

    @Override
    public int getItemCount() {
        return mItemHelper.getItemCount();
    }

    @Override
    public int getItemViewType(int position) {
        return position;
    }

    class VH extends RecyclerView.ViewHolder {

        public VH(View itemView) {
            super(itemView);
        }
    }


}
