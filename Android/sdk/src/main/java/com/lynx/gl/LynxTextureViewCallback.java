package com.lynx.gl;

import android.graphics.SurfaceTexture;
import android.support.annotation.Keep;
import android.util.Log;
import android.view.Surface;
import android.view.TextureView;

/**
 * Created by dli on 2018/3/8.
 */

public class LynxTextureViewCallback implements TextureView.SurfaceTextureListener {
    private Surface mSurface;
    private long mNativeRenderer;

    LynxTextureViewCallback() {
        mNativeRenderer = nativeOnTextureViewCreate();
    }

    @Override
    public void onSurfaceTextureAvailable(SurfaceTexture surface, int width, int height) {
        if(mSurface == null) {
            mSurface = new Surface(surface);
        }
        nativeOnSurfaceTextureAvailable(mNativeRenderer, mSurface, width, height);

        Log.e("lynx", "onSurfaceTextureAvailable");
    }

    @Override
    public void onSurfaceTextureSizeChanged(SurfaceTexture surface, int width, int height) {
        Log.e("lynx", "onSurfaceTextureSizeChanged");
    }

    @Override
    public boolean onSurfaceTextureDestroyed(SurfaceTexture surface) {
        Log.e("lynx", "onSurfaceTextureDestroyed");
        return false;
    }

    @Override
    public void onSurfaceTextureUpdated(SurfaceTexture surface) {
        Log.e("lynx", "onSurfaceTextureUpdated");
    }

    @Keep
    private static native long nativeOnTextureViewCreate();

    @Keep
    public static native void nativeOnSurfaceTextureAvailable(long renderer, Surface surface, int width, int height);
}
