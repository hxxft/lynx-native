package com.lynx.ui.canvas;

import android.content.Context;

import com.lynx.core.base.LynxArray;
import com.lynx.core.impl.RenderObjectAttr;
import com.lynx.core.impl.RenderObjectImpl;
import com.lynx.ui.LynxUI;


public class LynxUICanvas extends LynxUI<AndroidCanvas> {

    public LynxArray appendDrawCmds;
    private LynxArray canvasCmds;


    public LynxUICanvas(Context context, RenderObjectImpl impl) {
        super(context, impl);
    }

    @Override
    protected AndroidCanvas createView(Context context) {
        return new AndroidCanvas(context, this);
    }


    @Override
    public void setData(int attr, Object param) {
        super.setData(attr, param);
        if(attr == RenderObjectAttr.CANVAS_DRAW.value()) {
            canvasCmds = (LynxArray)param;
            ((AndroidCanvas)this.getView()).render();
        }else if(attr == RenderObjectAttr.CANVAS_APPEND.value()){
            appendDrawCmds = (LynxArray)param;
            ((AndroidCanvas)this.getView()).updateRender();
        }
    }

     LynxArray getCanvasCmds(boolean isAppend){
        if(isAppend){
            if(canvasCmds == null){
                canvasCmds = appendDrawCmds;
            }else{
                canvasCmds.addAll(appendDrawCmds);
            }
            LynxArray result = appendDrawCmds;
            appendDrawCmds = null;
            return result;
        }else{
            return canvasCmds;
       }
    }
}
