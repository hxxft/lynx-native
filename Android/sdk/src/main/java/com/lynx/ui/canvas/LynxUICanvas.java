package com.lynx.ui.canvas;

import android.content.Context;

import com.lynx.core.impl.RenderObjectImpl;
import com.lynx.ui.LynxUI;

import java.util.List;

/**
 * Created by chicheng on 2017/9/28.
 */

public class LynxUICanvas extends LynxUI<AndroidCanvas> {

    private static final String RENDER_CMD = "renderCmd";
    private List<String> cacheCanvasCmds;
    String canvasCmds;


    public LynxUICanvas(Context context, RenderObjectImpl impl) {
        super(context, impl);
    }

    @Override
    protected AndroidCanvas createView(Context context) {
        return new AndroidCanvas(context, this);
    }

    @Override
    public void setAttribute(String key, String value) {
        super.setAttribute(key, value);
        if(RENDER_CMD.equals(key)){
            canvasCmds = value;
            this.getView().invalidate();
        }
    }
}
