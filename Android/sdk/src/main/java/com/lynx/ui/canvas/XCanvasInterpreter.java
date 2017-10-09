package com.lynx.ui.canvas;

import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Matrix;
import android.graphics.Paint;
import android.graphics.Path;
import android.graphics.PorterDuff;
import android.graphics.PorterDuffXfermode;
import android.graphics.Rect;
import android.graphics.RectF;
import android.graphics.drawable.Drawable;
import android.text.TextUtils;
import android.util.Log;
import android.util.LruCache;
import android.view.View;

import com.lynx.core.base.LynxArray;
import com.lynx.core.base.LynxObject;
import com.lynx.core.impl.RenderObjectAttr;
import com.squareup.picasso.Picasso;
import com.squareup.picasso.Target;


public class XCanvasInterpreter {

    // 执行绘制时的画笔和画布
    private Paint mDrawPaint;
    private Canvas mDrawCanvas;

    // for final draw
    private Paint mPaint;

    // 因为图像混合，可能出现多支画笔和画布，所以这里用来保存最原始的
    private Paint mOriginPaint;
    private Canvas mOriginCanvas;

    // 上一次的bitmap，在有append命令时使用
    private Bitmap mCacheBitMap;

    private Path mPath;
    private int mFillStyle = android.graphics.Color.BLACK;
    private int mStrokeStyle = android.graphics.Color.BLACK;
    private LruCache<String, Bitmap> mImageCache = new LruCache<>(8);

    // 当使用混合模式的时候需要绘制在新的bitmap上
    private boolean needBitMapCanvas = false;
    private Bitmap canvasBitmap;

    public XCanvasInterpreter() {
    }


    public void exe(LynxUICanvas mUI, Canvas mCanvas, View mImpl, boolean isAppend) {
        if(mDrawPaint == null || !isAppend){
            mDrawPaint = new Paint();
            mDrawPaint.setAntiAlias(true);
            mDrawPaint.setColor(android.graphics.Color.BLACK);
            needBitMapCanvas = false;
        }
        // 将该画笔也保存一份
        mOriginPaint = mDrawPaint;
        // 这里打算每次都先画在一个bitmap上，最后绘制到父view的canvas上，这样能对bitmap进行保存
        Canvas mParentCanvas = mCanvas;


        // 如果是新加，则将lastBitmap先画上去
        if(isAppend && mCacheBitMap !=null){
            mOriginCanvas = new Canvas(mCacheBitMap);
        }else{
            // 创建一个用于绘制的bitmap，根据该bitmap来创建用于绘制的canvas
            if(mCacheBitMap == null){
                mCacheBitMap = Bitmap.createBitmap(mCanvas.getWidth(), mCanvas.getHeight(), Bitmap.Config.ARGB_8888);
            }
            mOriginCanvas = new Canvas(mCacheBitMap);
            mOriginCanvas.drawColor(android.graphics.Color.TRANSPARENT, PorterDuff.Mode.CLEAR);
        }
        this.mDrawCanvas = mOriginCanvas;
        if(null == mUI){
            return;
        }
        LynxArray canvasCmds = mUI.getCanvasCmds(isAppend);
        if(null == canvasCmds){
            return;
        }
        int length = canvasCmds.length();
        for(int i = 0; i<length; i++){
            exeSingleCmd((LynxObject) canvasCmds.get(i), mImpl);
        }
        // 最后将生成的mOriginBitMap绘制到mParentCanvas上
        if(mPaint == null){
            mPaint = new Paint();
        }
        mParentCanvas.drawBitmap(mCacheBitMap, 0, 0, mPaint );

        // 将这个图片数据传回
        mUI.updateData(RenderObjectAttr.CANVAS_IMAGE_DATA, mCacheBitMap);
    }

    private void exeSingleCmd(LynxObject cmdObject, View mImpl) {
        String type = (String) cmdObject.getProperty("type");
        switch (type){
            case "fr":
                fillRect(cmdObject);
                break;
            case "sr":
                strokeRect(cmdObject);
                break;
            case "cr":
                clearRect(cmdObject);
                break;
            case "fs":
                setFillStyle(cmdObject);
                break;
            case "ss":
                setStrokeStyle(cmdObject);
                break;
            case "di":
                drawImage(cmdObject,mImpl);
                break;
            case "bp":
                beginPath();
                break;
            case "cp":
                closePath();
                break;
            case "mt":
                moveTo(cmdObject);
                break;
            case "lt":
                lineTo(cmdObject);
                break;
            case "arc":
                arc(cmdObject);
                break;
            case "ft":
                mDrawPaint.setStyle(Paint.Style.FILL);
                mDrawPaint.setColor(mFillStyle);
                drawText(cmdObject);
                break;
            case "lw":
                setLineWidth(cmdObject);
                break;
            case "sf":
                setFont(cmdObject);
                break;
            case "tf":
                transform(cmdObject);
                break;
            case "stf":
                setTransform(cmdObject);
                break;
            case "tl":
                translate(cmdObject);
                break;
            case "rotate":
                rotate(cmdObject);
                break;
            case "scale":
                scale(cmdObject);
                break;
            case "fill":
                fill();
                break;
            case "stroke":
                stroke();
                break;
            case "lc":
                setLineCap(cmdObject);
                break;
            case "lj":
                setLineJoin(cmdObject);
                break;
            case "st":
                mDrawPaint.setStyle(Paint.Style.STROKE);
                mDrawPaint.setColor(mStrokeStyle);
                drawText(cmdObject);
                break;
            case "ta":
                setTextAlign(cmdObject);
                break;
            case "gco":
                setGlobalCompositeOperation(cmdObject);
                break;
            case "qct":
                quadraticCurveTo(cmdObject);
                break;
            case "bct":
                bezierCurveTo(cmdObject);
                break;
            case "save":
                save();
                break;
            case "restore":
                restore();
                break;
        }
    }

    /**
     * 三阶贝塞尔曲线
     */
    private void bezierCurveTo(LynxObject param) {
        mPath.cubicTo((float)param.getProperty("cp1x"),(float)param.getProperty("cp1y"),
                (float)param.getProperty("cp2x"),(float)param.getProperty("cp2y"),
                (float)param.getProperty("x"),(float)param.getProperty("y"));
    }

    /**
     * 二阶贝塞尔曲线
     */
    private void quadraticCurveTo(LynxObject param) {
        mPath.quadTo((float)param.getProperty("cpx"),(float)param.getProperty("cpy"),
                (float)param.getProperty("x"),(float)param.getProperty("y"));
    }

    private void setTransform(LynxObject param) {
//        String[] attributes = getArray(param,",");
//        Matrix mMatrix = new Matrix();
//        mMatrix.setValues(new float[]{Float.parseFloat(attributes[0]),Float.parseFloat(attributes[2]),
//                Float.parseFloat(attributes[4]),Float.parseFloat(attributes[1]),
//                Float.parseFloat(attributes[3]),Float.parseFloat(attributes[5]),
//                0,0,1});
//        mDrawCanvas.setMatrix(mMatrix);
    }

    private void scale(LynxObject param) {
        mDrawCanvas.scale((float)param.getProperty("scalewidth"), (float)param.getProperty("scaleheight"));
    }

    private void rotate(LynxObject param) {
        mDrawCanvas.rotate((float) ((float)param.getProperty("angle") / Math.PI * 180));
    }

    private void translate(LynxObject param) {
        mDrawCanvas.translate((float)param.getProperty("x"), (float)param.getProperty("y"));
    }

    private void transform(LynxObject param) {
        Matrix mMatrix = new Matrix();
        mMatrix.setValues(new float[]{(float) param.getProperty("m11"), (float) param.getProperty("m12"),
                (float) param.getProperty("dx"), (float) param.getProperty("m21"),
                (float) param.getProperty("m22"), (float) param.getProperty("dy"),
                0,0,1});
        mDrawCanvas.concat(mMatrix);
    }

    private void restore() {
        mDrawCanvas.restore();
    }

    private void save() {
        mDrawCanvas.save();
    }

    private void drawImage(LynxObject param, final View mImpl) {
        float x = (float) param.getProperty("x");
        float y = (float) param.getProperty("y");
        float width = (float) param.getProperty("width");
        float height = (float) param.getProperty("height");
        final String img = (String) param.getProperty("img");

        if(null != mImageCache.get(img)){
            if(param.getPropertyNames().size() == 6){
                mDrawCanvas.drawBitmap(mImageCache.get(img), null, new RectF(x,
                        y, x+width,
                        y+height), mDrawPaint);
            }else if(param.getPropertyNames().size() == 10){
                float sx = (float) param.getProperty("sx");
                float sy = (float) param.getProperty("sy");
                float swidth = (float) param.getProperty("swidth");
                float sheight = (float) param.getProperty("sheight");
                mDrawCanvas.drawBitmap(mImageCache.get(img), new Rect((int)sx, (int)sy,(int)(sx+swidth),
                        (int)(sy+sheight)),new RectF(x, y, x+width,
                        y+height),mDrawPaint);
            }

            return;
        }

        // 因为图片会被切割，为了下载原图直接使用picasso
        Target mTarget = new Target() {
            @Override
            public void onBitmapLoaded(Bitmap bitmap, Picasso.LoadedFrom from) {
                Log.i("ccc", "onBitmapLoaded");
                if(null == mImageCache.get(img)){
                    mImageCache.put(img,bitmap);
                    mImpl.invalidate();
                }
            }
            @Override
            public void onBitmapFailed(Drawable errorDrawable) {
            }

            @Override
            public void onPrepareLoad(Drawable placeHolderDrawable) {
            }
        };
        Picasso.with(mImpl.getContext()).load(img).into(mTarget);

    }

    private void setFont(LynxObject param) {
        String value = (String) param.getProperty("value");
        String[] fontParams = value.split(" ");
        mDrawPaint.setTextSize(Float.parseFloat(fontParams[0].replace("px", "")));
    }

    private void drawText(LynxObject param) {
        mDrawCanvas.drawText((String)param.getProperty("text"), (float)param.getProperty("x") ,
                (float)param.getProperty("y") ,mDrawPaint);
    }

    private void closePath() {
        if(null != mPath){
            mPath.close();
        }
    }

    private void setFillStyle(LynxObject param){
        mFillStyle = (Integer) param.getProperty("value");
        mDrawPaint.setColor(mFillStyle);
    }

    private void setStrokeStyle(LynxObject param){
        mStrokeStyle = (Integer) param.getProperty("value");
        mDrawPaint.setColor(mStrokeStyle);
    }

    private void clearRect(LynxObject param) {
    // todo 暂未实现
    }

    private void strokeRect(LynxObject param) {
        mDrawPaint.setStyle(Paint.Style.STROKE);
        mDrawPaint.setColor(mStrokeStyle);
        mDrawCanvas.drawRect( (float)(param.getProperty("x")), (float)(param.getProperty("y")),
                (float)(param.getProperty("width"))+(float)(param.getProperty("x")),
                (float)(param.getProperty("height"))+(float)(param.getProperty("y")), mDrawPaint);
    }

    private void fillRect(LynxObject param) {
        mDrawPaint.setStyle(Paint.Style.FILL);
        mDrawPaint.setColor(mFillStyle);
        mDrawCanvas.drawRect( (float)(param.getProperty("x")), (float)(param.getProperty("y")),
                (float)(param.getProperty("width"))+(float)(param.getProperty("x")),
                (float)(param.getProperty("height"))+(float)(param.getProperty("y")), mDrawPaint);
    }
    private void beginPath() {
        mPath = new Path();
    }
    private void moveTo(LynxObject param) {
        if(null != mPath){
            mPath.moveTo((float)(param.getProperty("x")), (float)(param.getProperty("y")));
        }
    }
    private void lineTo(LynxObject param) {
        if(null != mPath){
            mPath.lineTo((float)(param.getProperty("x")), (float)(param.getProperty("y")));
        }
    }
    //left,top,right,bottom,startAngle,sweepAngle
    private void arc(LynxObject param) {
        if(null != mPath){
            float x = (float) param.getProperty("x");
            float y = (float) param.getProperty("y");
            float radius = (float) param.getProperty("r");
            float startAngle = (float) param.getProperty("sAngle");
            float endAngle =(float) param.getProperty("eAngle");
            boolean anticlockwise = (boolean) param.getProperty("counterclockwise");

            double sweepAngle = endAngle - startAngle;
            double sweepDegress = sweepAngle * 180 / Math.PI % 360;
            if (anticlockwise) {
                sweepDegress = sweepDegress - 360;
            }
            double startDegress = startAngle * 180 / Math.PI % 360;
            double right = x + radius;
            double bottom = y + radius;
            double left = x - radius;
            double top = y - radius;
            mPath.addArc(new RectF((float)left,(float)top,(float)right,(float)bottom), (float)startDegress, (float)sweepDegress);
        }
    }
    private void fill(){
        if(null != mPath){
            mDrawPaint.setStyle(Paint.Style.FILL);
            mDrawPaint.setColor(mFillStyle);
            mDrawCanvas.drawPath(mPath, mDrawPaint);
        }
    }


    private void stroke(){
        if(null != mPath){
        mDrawPaint.setStyle(Paint.Style.STROKE);
        mDrawPaint.setColor(mStrokeStyle);
        mDrawCanvas.drawPath(mPath, mDrawPaint);
    }
    }

    private void setLineWidth(LynxObject params) {
        mDrawPaint.setStrokeWidth((int) params.getProperty("value"));
    }

    private void setLineCap(LynxObject params) {
        String value = (String) params.getProperty("value");
        if(TextUtils.isEmpty(value)) return;
        switch (value){
            case "butt":
                mDrawPaint.setStrokeCap(Paint.Cap.BUTT);
                break;
            case "square":
                mDrawPaint.setStrokeCap(Paint.Cap.SQUARE);
                break;
            case "round":
                mDrawPaint.setStrokeCap(Paint.Cap.ROUND);
                break;

        }
    }

    private void setTextAlign(LynxObject params) {
        String value = (String) params.getProperty("value");
        if(TextUtils.isEmpty(value)) return;
        switch (value){
            case "center":
                mDrawPaint.setTextAlign(Paint.Align.CENTER);
                break;
            case "end":
                mDrawPaint.setTextAlign(Paint.Align.RIGHT);
                break;
            case "left":
                mDrawPaint.setTextAlign(Paint.Align.LEFT);
                break;
            case "right":
                mDrawPaint.setTextAlign(Paint.Align.RIGHT);
                break;
            case "start":
                mDrawPaint.setTextAlign(Paint.Align.LEFT);
                break;
        }
    }

    private void setLineJoin(LynxObject params) {
        String value = (String) params.getProperty("value");
        if(TextUtils.isEmpty(value)) return;
        switch (value){
            case "bevel":
                mDrawPaint.setStrokeJoin(Paint.Join.BEVEL);
                break;
            case "miter":
                mDrawPaint.setStrokeJoin(Paint.Join.MITER);
                break;
            case "round":
                mDrawPaint.setStrokeJoin(Paint.Join.ROUND);
                break;
        }
    }

    /**
     * 当用到混合模式的时候，需要先将图绘在bitmap上再绘到我们的真正的canvas上
     */
    private void createNewCanvas() {
        canvasBitmap = Bitmap.createBitmap(mOriginCanvas.getWidth(),mOriginCanvas.getHeight(), Bitmap.Config.ARGB_8888);
        mDrawCanvas = new Canvas(canvasBitmap);
        mDrawPaint = new Paint(mOriginPaint);
        mDrawPaint.setXfermode(null);
    }

    /**
     * 绘制完了之后就准备重叠上去
     */
    private void mixCanvas() {
        if(needBitMapCanvas){
            mOriginCanvas.drawBitmap(canvasBitmap,0,0, mOriginPaint);
            canvasBitmap.recycle();
//            createNewCanvas();
        }
    }

    private void setGlobalCompositeOperation(LynxObject params) {
        if(needBitMapCanvas){
            mixCanvas();
        }
        String value = (String) params.getProperty("value");
        if(TextUtils.isEmpty(value)) return;
        switch (value){
            case "source-over":
                needBitMapCanvas = false;
                mDrawPaint.setXfermode(new PorterDuffXfermode(PorterDuff.Mode.SRC_OVER));
                break;
            case "source-atop":
                needBitMapCanvas = true;
                mDrawPaint.setXfermode(new PorterDuffXfermode(PorterDuff.Mode.SRC_ATOP));
                break;
            case "source-in":
                needBitMapCanvas = true;
                mDrawPaint.setXfermode(new PorterDuffXfermode(PorterDuff.Mode.SRC_IN));
                break;
            case "source-out":
                needBitMapCanvas = true;
                mDrawPaint.setXfermode(new PorterDuffXfermode(PorterDuff.Mode.SRC_OUT));
                break;
            case "destination-over":
                needBitMapCanvas = true;
                mDrawPaint.setXfermode(new PorterDuffXfermode(PorterDuff.Mode.DST_OVER));
                break;
            case "destination-atop":
                needBitMapCanvas = true;
                mDrawPaint.setXfermode(new PorterDuffXfermode(PorterDuff.Mode.DST_ATOP));
                break;
            case "destination-in":
                needBitMapCanvas = true;
                mDrawPaint.setXfermode(new PorterDuffXfermode(PorterDuff.Mode.DST_IN));
                break;
            case "destination-out":
                needBitMapCanvas = true;
                mDrawPaint.setXfermode(new PorterDuffXfermode(PorterDuff.Mode.DST_OUT));
                break;
            case "lighter":
                needBitMapCanvas = true;
                mDrawPaint.setXfermode(new PorterDuffXfermode(PorterDuff.Mode.LIGHTEN));
                break;
            case "copy":
//                mDrawPaint.setXfermode(new PorterDuffXfermode(PorterDuff.Mode.ADD));
                break;
            case "xor":
                needBitMapCanvas = true;
                mDrawPaint.setXfermode(new PorterDuffXfermode(PorterDuff.Mode.XOR));
                break;
            case "null":
                needBitMapCanvas = false;
                mDrawPaint.setXfermode(null);
                break;
            case "clear":
                needBitMapCanvas = true;
                mDrawPaint.setXfermode(new PorterDuffXfermode(PorterDuff.Mode.CLEAR));
                break;
        }
        if(needBitMapCanvas){
            createNewCanvas();
        }else{
            mDrawCanvas = mOriginCanvas;
            mDrawPaint = mOriginPaint;
            mOriginPaint.setXfermode(new PorterDuffXfermode(PorterDuff.Mode.SRC_OVER));
        }
    }
}
