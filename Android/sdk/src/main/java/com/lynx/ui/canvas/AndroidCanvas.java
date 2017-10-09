package com.lynx.ui.canvas;

import android.content.Context;
import android.graphics.Canvas;
import android.view.View;


public class AndroidCanvas extends View {


    private XCanvasInterpreter mInterpreter;
    private LynxUICanvas mUI;
    private boolean isAppendDraw = false;

    public AndroidCanvas(Context context) {
        super(context);
    }

    public AndroidCanvas(Context context, LynxUICanvas lynxUICanvas) {
        super(context);
        mUI = lynxUICanvas;
        mInterpreter = new XCanvasInterpreter();
    }

    @Override
    protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
        setMeasuredDimension(
                MeasureSpec.getSize(widthMeasureSpec),
                MeasureSpec.getSize(heightMeasureSpec));
    }

    @Override
    protected void onLayout(boolean changed,
                            int l, int t, int r, int b) {
    }

    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);
        mInterpreter.exe(mUI, canvas, this, isAppendDraw);
    }

    public void render() {
        isAppendDraw = false;
        invalidate();
    }

    public void updateRender() {
        isAppendDraw = true;
        invalidate();
    }
}
