package com.lynx.ui.canvas;

import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Matrix;
import android.graphics.Paint;
import android.graphics.Path;
import android.graphics.Rect;
import android.graphics.RectF;
import android.graphics.drawable.Drawable;
import android.util.Log;
import android.util.LruCache;
import android.view.View;

import com.squareup.picasso.Picasso;
import com.squareup.picasso.Target;

import java.util.StringTokenizer;

/**
 * Created by chicheng on 2017/9/13.
 */

public class XCanvasInterpreter {

    private Paint mPaint;
    private Canvas mCanvas;
    private Path mPath;
    private LruCache<String, Bitmap> mImageCache = new LruCache<>(8);

    public XCanvasInterpreter() {
    }


    public void exe(String canvasCmds, Canvas mCanvas, View mImpl) {
        mPaint = new Paint();
        mPaint.setAntiAlias(true);
        mPaint.setStrokeCap(Paint.Cap.ROUND);
        mPaint.setTextSize(12);
//        mPaint.setColor(0x000220);
        this.mCanvas = mCanvas;
        if(canvasCmds == null){
            return;
        }
        StringTokenizer st = new StringTokenizer(canvasCmds, "^");
        while (st.hasMoreTokens()){
            exeSingleCmd(st.nextToken(), mImpl);
        }
    }

    private void exeSingleCmd(String cmd, View mImpl) {
        String[] params = getArray(cmd,"@");
        switch (params[0]){
            case "fr":
                fillRect(params[1]);
                break;
            case "sr":
                strokeRect(params[1]);
                break;
            case "cr":
                clearRect(params[1]);
                break;
            case "fs":
                setFillStyle(params[1]);
                break;
            case "di":
                drawImage(params[1],mImpl);
                break;
            case "bp":
                beginPath();
                break;
            case "cp":
                closePath();
                break;
            case "mt":
                moveTo(params[1]);
                break;
            case "lt":
                lineTo(params[1]);
                break;
            case "arc":
                arc(params[1]);
                break;
            case "ft":
                mPaint.setStyle(Paint.Style.FILL);
                drawText(params[1]);
                break;
            case "lw":
                setLineWidth(params[1]);
                break;
            case "sf":
                setFont(params[1]);
                break;
            case "tf":
                transform(params[1]);
                break;
            case "stf":
                setTransform(params[1]);
                break;

            case "tl":
                translate(params[1]);
                break;
            case "rotate":
                rotate(params[1]);
                break;
            case "scale":
                scale(params[1]);
                break;
            case "dis":
                drawImageS(params[1], mImpl);
                break;
            case "fill":
                fill();
                break;
            case "stroke":
                stroke();
                break;
            case "save":
                save();
                break;
            case "restore":
                restore();
                break;
        }
    }



    private void setTransform(String param) {
        String[] attributes = getArray(param,",");
        Matrix mMatrix = new Matrix();
        mMatrix.setValues(new float[]{Float.parseFloat(attributes[0]),Float.parseFloat(attributes[2]),
                Float.parseFloat(attributes[4]),Float.parseFloat(attributes[1]),
                Float.parseFloat(attributes[3]),Float.parseFloat(attributes[5]),
                0,0,1});
        mCanvas.setMatrix(mMatrix);
    }

    private void scale(String param) {
        String[] attributes = getArray(param,",");
        mCanvas.scale(Integer.parseInt(attributes[0])/100f,Integer.parseInt(attributes[1])/100f);
    }

    private void rotate(String param) {
        mCanvas.rotate((float) (Integer.parseInt(param)/100f / Math.PI * 180));
    }

    private void translate(String param) {
        String[] attributes = getArray(param,",");
        mCanvas.translate(Integer.parseInt(attributes[0])/100f,Integer.parseInt(attributes[1])/100f);
    }

    private void transform(String param) {
        String[] attributes = getArray(param,",");
        Matrix mMatrix = new Matrix();
        mMatrix.setValues(new float[]{Integer.parseInt(attributes[0])/100f,Integer.parseInt(attributes[2])/100f,
                Integer.parseInt(attributes[4])/100f,Integer.parseInt(attributes[1])/100f,
                Integer.parseInt(attributes[3])/100f,Integer.parseInt(attributes[5])/100f,
                0,0,1});
        mCanvas.concat(mMatrix);
    }

    private void restore() {
        mCanvas.restore();
    }

    private void save() {
        mCanvas.save();
    }

    private void drawImage(String param, final View mImpl) {
        final String[] attributes = getArray(param,",");
        if(null != mImageCache.get(attributes[0])){
            mCanvas.drawBitmap(mImageCache.get(attributes[0]), null, new RectF(Integer.parseInt(attributes[1])/100f,
                    Integer.parseInt(attributes[2])/100f, Integer.parseInt(attributes[3])/100f,
                    Integer.parseInt(attributes[4])/100f), mPaint);
            return;
        }

        // 因为图片会被切割，为了下载原图直接使用picasso
        Target mTarget = new Target() {
            @Override
            public void onBitmapLoaded(Bitmap bitmap, Picasso.LoadedFrom from) {
                Log.i("ccc", "onBitmapLoaded");
                if(null == mImageCache.get(attributes[0])){
                    mImageCache.put(attributes[0],bitmap);
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
        Picasso.with(mImpl.getContext()).load(attributes[0]).into(mTarget);

    }
    private void drawImageS(String param, final View mImpl) {
        final String[] attributes = getArray(param,",");
        if(null != mImageCache.get(attributes[0])){
            mCanvas.drawBitmap(mImageCache.get(attributes[0]), new Rect(Integer.parseInt(attributes[1])/100,
                    Integer.parseInt(attributes[2])/100, Integer.parseInt(attributes[3])/100+Integer.parseInt(attributes[1])/100,
                    Integer.parseInt(attributes[4])/100+Integer.parseInt(attributes[2])/100),new RectF(Integer.parseInt(attributes[5])/100f,
                    Integer.parseInt(attributes[6])/100f, Integer.parseInt(attributes[7])/100f,
                    Integer.parseInt(attributes[8])/100f),mPaint);
            return;
        }
        // 因为图片会被切割，为了下载原图直接使用picasso
        Target mTarget = new Target() {
            @Override
            public void onBitmapLoaded(Bitmap bitmap, Picasso.LoadedFrom from) {
                Log.i("ccc", "onBitmapLoaded");
                if(null == mImageCache.get(attributes[0])){
                    mImageCache.put(attributes[0],bitmap);
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
        Picasso.with(mImpl.getContext()).load(attributes[0]).into(mTarget);

    }

    private void setFont(String param) {
        String[] attributes = getArray(param,",");
        mPaint.setTextSize(Float.parseFloat(attributes[0]));
    }

    private void drawText(String param) {
        String[] attributes = getArray(param,",");
        mCanvas.drawText(attributes[0], Integer.parseInt(attributes[1])/100f ,
                Integer.parseInt(attributes[2])/100f ,mPaint);
    }

    private void closePath() {
        if(null != mPath){
            mPath.close();
        }
    }

    private void setFillStyle(String param){
        mPaint.setColor(Color.parseColor(param));
    }

    private void clearRect(String param) {
    // todo 暂未实现
    }

    private void strokeRect(String param) {
        mPaint.setStyle(Paint.Style.STROKE);
        String[] attributes = getArray(param,",");
        mCanvas.drawRect( Integer.parseInt(attributes[0])/100f,Integer.parseInt(attributes[1])/100f,
                Integer.parseInt(attributes[2])/100f,Integer.parseInt(attributes[3])/100f,mPaint);
    }

    private void fillRect(String param) {
        mPaint.setStyle(Paint.Style.FILL);
        String[] attributes = getArray(param,",");
        mCanvas.drawRect( Integer.parseInt(attributes[0])/100f,Integer.parseInt(attributes[1])/100f,
                Integer.parseInt(attributes[2])/100f,Integer.parseInt(attributes[3])/100f,mPaint);
    }
    private void beginPath() {
        mPath = new Path();
    }
    private void moveTo(String param) {
        if(null != mPath){
            String[] attributes = getArray(param,",");
            mPath.moveTo(Integer.parseInt(attributes[0])/100f,Integer.parseInt(attributes[1])/100f);
        }
    }
    private void lineTo(String param) {
        if(null != mPath){
            String[] attributes = getArray(param,",");
            mPath.lineTo(Integer.parseInt(attributes[0])/100f,Integer.parseInt(attributes[1])/100f);
        }
    }
    //left,top,right,bottom,startAngle,sweepAngle
    private void arc(String param) {
        if(null != mPath){
            String[] attributes = getArray(param,",");
            double x = Integer.parseInt(attributes[0])/100d;
            double y = Integer.parseInt(attributes[1])/100d;
            double radius = Integer.parseInt(attributes[2])/100d;
            double startAngle = Integer.parseInt(attributes[3])/100d;
            double endAngle = Integer.parseInt(attributes[4])/100d;
            boolean anticlockwise = Boolean.parseBoolean(attributes[5]);
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
            mPaint.setStyle(Paint.Style.FILL);
            mCanvas.drawPath(mPath, mPaint);
            mPath = null;
        }
    }


    private void stroke(){
        if(null != mPath){
            mPaint.setStyle(Paint.Style.STROKE);
            mCanvas.drawPath(mPath, mPaint);
            mPath = null;
        }
    }

    private void setLineWidth(String lineWidth) {
        mPaint.setStrokeWidth(Integer.parseInt(lineWidth)/100f);
    }
    private String[] getArray(String param, String demiliter){
        if(null == demiliter){
            demiliter = ",";
        }
        StringTokenizer st = new StringTokenizer(param, demiliter);
        String[] a = new String[st.countTokens()];
        for(int i=0;i < a.length;i++){
            a[i] = st.nextToken();
        }
        return a;
    }
}
