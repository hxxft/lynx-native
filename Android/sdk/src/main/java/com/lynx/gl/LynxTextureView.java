package com.lynx.gl;

import android.annotation.TargetApi;
import android.content.Context;
import android.os.Build;
import android.util.AttributeSet;
import android.util.Log;
import android.view.TextureView;

/**
 * Created by dli on 2018/3/8.
 */

public class LynxTextureView extends TextureView{
    public LynxTextureView(Context context) {
        super(context);
        init();
        Log.d("lynx", "LynxTextureView");
    }

    public LynxTextureView(Context context, AttributeSet attrs) {
        super(context, attrs);
        init();
    }

    public LynxTextureView(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        init();
    }

    @TargetApi(Build.VERSION_CODES.LOLLIPOP)
    public LynxTextureView(Context context, AttributeSet attrs, int defStyleAttr, int defStyleRes) {
        super(context, attrs, defStyleAttr, defStyleRes);
        init();
    }

    private void init() {
        this.setSurfaceTextureListener(new LynxTextureViewCallback());
        this.setOpaque(false);
        this.setLayerType(LAYER_TYPE_HARDWARE, null);
    }
}
