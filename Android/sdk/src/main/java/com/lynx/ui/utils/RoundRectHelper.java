// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.ui.utils;

import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.PorterDuff;
import android.graphics.PorterDuffXfermode;
import android.graphics.RectF;
import android.util.Log;
import android.util.SparseArray;

import com.lynx.core.impl.RenderObjectImpl;

public class RoundRectHelper {
    private static int TOO_LARGE_VALUE = 5000;
    private static MaskKeeper sMaskKeeper;
    private static RoundRectHelper sHelper;

    private PorterDuffXfermode mMode = new PorterDuffXfermode(PorterDuff.Mode.DST_IN);

    public static synchronized void clearMasks() {
        if (sMaskKeeper != null) {
            sMaskKeeper.clearAll();
            sMaskKeeper = null;
            sHelper = null;
        }
    }

    public static synchronized RoundRectHelper instance() {
        if (sHelper == null) {
            sHelper = new RoundRectHelper();
        }
        return sHelper;
    }

    private RoundRectHelper() {
        if (sMaskKeeper == null) {
            sMaskKeeper = new MaskKeeper(5);
        }
    }

    static boolean isTooLarge(double a) {
        boolean result = a > TOO_LARGE_VALUE;
        if (result) {
            Log.w("lynx", "Image Mask maybe too large, it isn't suggested !");
        }
        return result;
    }

    public void clipRoundRectWithXfermode(Canvas canvas, RenderObjectImpl impl) {
        clipRoundRectWithXfermode(canvas, impl, null);
    }

    public void clipRoundRectWithXfermode(Canvas canvas, RenderObjectImpl impl, Bitmap.Config config) {
        if (impl == null) {
            return;
        }
        int width = impl.getPosition().getWidth();
        int height = impl.getPosition().getHeight();
        clipRoundRectWithXfermode(canvas, width, height,
                (float) impl.getStyle().mBorderRadius,
                (float) impl.getStyle().mBorderRadius, config);
    }

    public void clipRoundRectWithXfermode(Canvas canvas, int width, int height,
                                          float rx, float ry, Bitmap.Config config) {
        if (width == 0 || height == 0 ||
                Double.isInfinite(width) || Double.isNaN(width) ||
                Double.isInfinite(height) || Double.isNaN(height) ||
                isTooLarge(width) || isTooLarge(height)) {
            return;
        }

        Bitmap mask = null;
        boolean needToRecycle = false;
        Paint maskPaint = new Paint(Paint.ANTI_ALIAS_FLAG);

        int key = generateKey(width, height, rx, ry);
        if (config == null) {
            mask = sMaskKeeper.get(key);
        }
        if (mask == null) {
            if (config == null) {
                mask = Bitmap.createBitmap(width, height, Bitmap.Config.ALPHA_8);
                needToRecycle = false;
            } else {
                mask = Bitmap.createBitmap(width, height, config);
                needToRecycle = true;
            }
            Canvas maskCanvas = new Canvas();
            maskCanvas.setBitmap(mask);
            maskCanvas.drawRoundRect(new RectF(0, 0, width, height), rx, ry, maskPaint);
            if (!needToRecycle) {
                sMaskKeeper.set(key, mask);
            }
        }

        if (mask != null && !mask.isRecycled()) {
            maskPaint.setXfermode(mMode);
            canvas.drawBitmap(mask, 0, 0, maskPaint);
            if (needToRecycle) {
                mask.recycle();
            }
        }
    }

    private int generateKey(int width, int height, float rx, float ry) {
        int hash = 17;
        hash = hash * 31 + width;
        hash = hash * 31 + height;
        hash = hash * 31 + Float.floatToIntBits(rx);
        hash = hash * 31 + Float.floatToIntBits(ry);
        return hash;
    }

    /* LRU Cache */
    private class MaskKeeper {
        private int mCapacity;
        private int mCount;
        private SparseArray<Pair> mMaps;
        private Pair mHead, mTail;

        MaskKeeper(int capacity) {
            this.mCapacity = capacity;
            mMaps = new SparseArray<>();
            mHead = new Pair(0, null);
            mTail = new Pair(-1, null);
            mHead.last = null;
            mHead.next = mTail;
            mTail.last = mHead;
            mTail.next = null;
        }

        synchronized void clearAll() {
            for (int i = 0; i < mMaps.size(); ++i) {
                Pair pair = mMaps.valueAt(i);
                if (pair.mask != null && !pair.mask.isRecycled()) {
                    pair.mask.recycle();
                }
            }
            mMaps.clear();
        }

        public synchronized Bitmap get(int key) {
            Pair pair = mMaps.get(key);
            if (pair != null) {
                moveToFirst(pair);
                return pair.mask;
            }
            return null;
        }

        public synchronized void set(int key, Bitmap mask) {

            if (mMaps.get(key) == null) {
                Pair pair = new Pair(key, mask);
                mCount++;
                addPair(pair);
                mMaps.put(pair.key, pair);
                if (mCount > mCapacity) {
                    Pair removed = popTail();
                    mMaps.remove(removed.key);
                    mCount--;
                }
            } else {
                Pair pair = mMaps.get(key);
                pair.mask = mask;
                moveToFirst(pair);
            }
        }

        private void addPair(Pair pair) {
            pair.last = mHead;
            pair.next = mHead.next;

            mHead.next.last = pair;
            mHead.next = pair;
        }

        private void removePair(Pair pair) {
            Pair last = pair.last;
            Pair next = pair.next;

            last.next = next;
            next.last = last;
        }

        private void moveToFirst(Pair pair) {
            removePair(pair);
            addPair(pair);
        }

        private Pair popTail() {
            Pair pop = mTail.last;
            removePair(pop);
            return pop;
        }
    }

    private class Pair {
        int key;
        Bitmap mask;
        Pair next;
        Pair last;

        public Pair(int key, Bitmap mask) {
            this.key = key;
            this.mask = mask;
        }

        @Override
        public boolean equals(Object obj) {
            if (obj == null) {
                return false;
            }
            if (obj instanceof Pair && ((Pair) obj).key == this.key) {
                return true;
            }
            return false;
        }
    }
}
