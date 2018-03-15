// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.test;

import android.widget.FrameLayout;

import com.lynx.utils.ScreenUtil;

import java.util.HashMap;
import java.util.LinkedHashMap;
import java.util.Map;

/**
 * 测试点击事件的 type 以及 touches 和 changedTouches 中的 clientX / clientY / screenX / screenY
 * 是否符合预期
 *
 * __________________
 * |   body         |
 * |   ________     |
 * |   |      |     |
 * |   | view |     |
 * |   |______|     |
 * |                |
 * |________________|
 */

public class TouchInfoTestCase extends BaseTestCase {
    private static final int DELAY_TIME = 200;
    private TestResultRecorder mRecorder;
    private float mRatio;

    public TouchInfoTestCase() {
    }

    @Override
    protected void setUp() throws Exception {
        super.setUp();
        mRatio = (float) ScreenUtil.getScreenWidth() / 750;
    }

    // 测试单点触控在子 view 情况，主要测试在单指情况下，type 以及 touches 和 changedTouches 中的
    // clientX / clientY / screenX / screenY 符合预期
    public void testSingleTouchInfoOnView() throws InterruptedException {
        Thread.sleep(DELAY_TIME);
        SingleTouchGenerator generator = new SingleTouchGenerator(getActivity());
        String tag = "view";
        float[] downXArray = new float[1];
        float[] downYArray = new float[1];
        // down -> move -> up
        downXArray[0] = 100;
        downYArray[0] = 100;
        generator.down(downXArray[0] * mRatio, downYArray[0] * mRatio);
        Thread.sleep(DELAY_TIME);
        assetTouchInfo(mRecorder.getResults(), tag, "touchstart", downXArray, downYArray, 1, 1);
        mRecorder.reset();
        Thread.sleep(DELAY_TIME);
        downXArray[0] = 101;
        downYArray[0] = 101;
        generator.move(downXArray[0] * mRatio, downYArray[0] * mRatio);
        Thread.sleep(DELAY_TIME);
        assetTouchInfo(mRecorder.getResults(), tag, "touchmove", downXArray, downYArray, 1, 1);
        mRecorder.reset();

        Thread.sleep(DELAY_TIME);
        generator.up();
        Thread.sleep(DELAY_TIME);
        assetTouchInfo(mRecorder.getResults(), tag, "touchend", downXArray, downYArray, 0, 1);
        mRecorder.reset();

        // down -> cancel
        downXArray[0] = 120;
        downYArray[0] = 120;
        generator.down(downXArray[0] * mRatio, downYArray[0] * mRatio);
        Thread.sleep(DELAY_TIME);
        assetTouchInfo(mRecorder.getResults(), tag, "touchstart", downXArray, downYArray, 1, 1);
        mRecorder.reset();

        Thread.sleep(DELAY_TIME);
        generator.cancel();
        Thread.sleep(DELAY_TIME);
        assetTouchInfo(mRecorder.getResults(), tag, "touchcancel", downXArray, downYArray, 0, 1);
        mRecorder.reset();

    }

    // 测试单点触控在 body 情况，主要测试在单指情况下，type 以及 touches 和 changedTouches 的数量和其中的
    // clientX / clientY / screenX / screenY 是否符合预期
    public void testSingleTouchInfoOnBody() throws InterruptedException {
        Thread.sleep(1000);
        SingleTouchGenerator generator = new SingleTouchGenerator(getActivity());
        String tag = "body";
        float[] downXArray = new float[1];
        float[] downYArray = new float[1];
        // down -> move -> up
        downXArray[0] = 20;
        downYArray[0] = 30;
        generator.down(downXArray[0] * mRatio, downYArray[0] * mRatio);
        Thread.sleep(DELAY_TIME);
        assetTouchInfo(mRecorder.getResults(), tag, "touchstart", downXArray, downYArray, 1, 1);
        mRecorder.reset();

        Thread.sleep(DELAY_TIME);
        downXArray[0] = 21;
        downYArray[0] = 29;
        generator.move(downXArray[0] * mRatio, downYArray[0] * mRatio);
        Thread.sleep(DELAY_TIME);
        assetTouchInfo(mRecorder.getResults(), tag, "touchmove", downXArray, downYArray, 1, 1);
        mRecorder.reset();

        Thread.sleep(DELAY_TIME);
        generator.up();
        Thread.sleep(DELAY_TIME);
        assetTouchInfo(mRecorder.getResults(), tag, "touchend", downXArray, downYArray, 0, 1);
        mRecorder.reset();

        // down -> cancel
        downXArray[0] = 10;
        downYArray[0] = 20;
        generator.down(downXArray[0] * mRatio, downYArray[0] * mRatio);
        Thread.sleep(DELAY_TIME);
        assetTouchInfo(mRecorder.getResults(), tag, "touchstart", downXArray, downYArray, 1, 1);
        mRecorder.reset();

        Thread.sleep(DELAY_TIME);
        generator.cancel();
        Thread.sleep(DELAY_TIME);
        assetTouchInfo(mRecorder.getResults(), tag, "touchcancel", downXArray, downYArray, 0, 1);
        mRecorder.reset();
    }

    // 测试当页面不是全屏时候，screenX / screenY / clientX / clientY 是否满足预期
    // 预期：screenX / screenY 和 clientX clientY 有区别，
    // clientX clientY 是相对于 body 的坐标
    // screenX / screenY 是相对于屏幕的坐标
    public void testScreenInfoOfTouch() throws Throwable {
        // 设置 body 偏移
        final int marginTop = 50;
        runTestOnUiThread(new Runnable() {
            @Override
            public void run() {
                ((FrameLayout.LayoutParams) getActivity().getLynxView().getLayoutParams())
                        .setMargins(0, (int) (marginTop * mRatio), 0, 0);
                getActivity().getLynxView().requestLayout();
            }
        });
        Thread.sleep(DELAY_TIME);

        SingleTouchGenerator generator = new SingleTouchGenerator(getActivity());
        String tag = "body";
        float[] downXArray = new float[1];
        float[] downYArray = new float[1];
        // down -> move -> up
        downXArray[0] = 20;
        downYArray[0] = 80;
        generator.down(downXArray[0] * mRatio, downYArray[0] * mRatio);
        Thread.sleep(DELAY_TIME);
        assetTouchInfo(mRecorder.getResults(), tag, "touchstart", 0, marginTop, downXArray, downYArray, 1, 1);
        mRecorder.reset();
    }

    // 测试多点触控在 view 的情况，重点关注 changedTouches 和 touches 的变化是否符合预期
    // down1 -> down2 -> move1 -> move2 -> move12 -> up2 -> up1
    public void testMultiTouchInfoOnView() throws InterruptedException {
        Thread.sleep(1000);
        MultiTouchGenerator generator = new MultiTouchGenerator(getActivity());

        float downX_1 = 100;
        float downY_1 = 100;
        float downX_2 = 200;
        float downY_2 = 100;

        // 用于设置期望值
        float[] touchesXArray = new float[2];
        float[] touchesYArray = new float[2];
        float[] changedTouchesXArray = new float[2];
        float[] changedTouchesYArray = new float[2];

        int pointerId_1 = generator.down(downX_1 * mRatio, downY_1 * mRatio);
        Thread.sleep(DELAY_TIME);
        // 设置期望值
        touchesXArray[0] = downX_1;
        touchesYArray[0] = downY_1;
        changedTouchesXArray[0] = touchesXArray[0];
        changedTouchesYArray[0] = touchesYArray[0];
        // 验证是否符合预期
        assetTouchInfo(mRecorder.getResults(), "view", "touchstart", 1, touchesXArray, touchesYArray,
                1, changedTouchesXArray, changedTouchesYArray);
        mRecorder.reset();
        Thread.sleep(DELAY_TIME);
        int pointerId_2 =  generator.down(downX_2 * mRatio, downY_2 * mRatio);
        Thread.sleep(DELAY_TIME);
        // 设置期望值
        touchesXArray[1] = downX_2;
        touchesYArray[1] = downY_2;
        changedTouchesXArray[0] = touchesXArray[1];
        changedTouchesYArray[0] = touchesYArray[1];
        // 验证期望值
        assetTouchInfo(mRecorder.getResults(), "view", "touchstart", 2, touchesXArray, touchesYArray,
                1, changedTouchesXArray, changedTouchesYArray);
        mRecorder.reset();

        // 验证 move
        Thread.sleep(DELAY_TIME);
        generator.move(new MultiTouchGenerator.MultiTouchMoveParam(pointerId_1, ++downX_1 * mRatio, ++downY_1 * mRatio));
        Thread.sleep(DELAY_TIME);
        touchesXArray[0] = downX_1;
        touchesYArray[0] = downY_1;
        touchesXArray[1] = downX_2;
        touchesYArray[1] = downY_2;
        changedTouchesXArray[0] = downX_1;
        changedTouchesYArray[0] = downY_1;
        assetTouchInfo(mRecorder.getResults(), "view", "touchmove", 2, touchesXArray, touchesYArray,
                1, changedTouchesXArray, changedTouchesYArray);
        mRecorder.reset();

        Thread.sleep(DELAY_TIME);
        generator.move(new MultiTouchGenerator.MultiTouchMoveParam(pointerId_2, ++downX_2 * mRatio, ++downY_2 * mRatio));
        Thread.sleep(DELAY_TIME);
        touchesXArray[0] = downX_1;
        touchesYArray[0] = downY_1;
        touchesXArray[1] = downX_2;
        touchesYArray[1] = downY_2;
        changedTouchesXArray[0] = downX_2;
        changedTouchesYArray[0] = downY_2;
        assetTouchInfo(mRecorder.getResults(), "view", "touchmove", 2, touchesXArray, touchesYArray,
                1, changedTouchesXArray, changedTouchesYArray);
        mRecorder.reset();

        Thread.sleep(DELAY_TIME);
        generator.move(new MultiTouchGenerator.MultiTouchMoveParam(pointerId_1, ++downX_1 * mRatio, ++downY_1 * mRatio),
                new MultiTouchGenerator.MultiTouchMoveParam(pointerId_2, ++downX_2 * mRatio, downY_2 * mRatio));
        Thread.sleep(DELAY_TIME);
        touchesXArray[0] = downX_1;
        touchesYArray[0] = downY_1;
        touchesXArray[1] = downX_2;
        touchesYArray[1] = downY_2;
        changedTouchesXArray[0] = downX_1;
        changedTouchesYArray[0] = downY_1;
        changedTouchesXArray[1] = downX_2;
        changedTouchesYArray[1] = downY_2;
        assetTouchInfo(mRecorder.getResults(), "view", "touchmove", 2, touchesXArray, touchesYArray,
                2, changedTouchesXArray, changedTouchesYArray);
        mRecorder.reset();

        Thread.sleep(DELAY_TIME);
        generator.up(pointerId_2);
        Thread.sleep(DELAY_TIME);
        touchesXArray[0] = downX_1;
        touchesYArray[0] = downY_1;
        changedTouchesXArray[0] = downX_2;
        changedTouchesYArray[0] = downY_2;
        assetTouchInfo(mRecorder.getResults(), "view", "touchend", 1, touchesXArray, touchesYArray,
                1, changedTouchesXArray, changedTouchesYArray);
        mRecorder.reset();
        Thread.sleep(DELAY_TIME);
        generator.up(pointerId_1);
        Thread.sleep(DELAY_TIME);
        changedTouchesXArray[0] = downX_1;
        changedTouchesYArray[0] = downY_1;
        assetTouchInfo(mRecorder.getResults(), "view", "touchend", 0, touchesXArray, touchesYArray,
                1, changedTouchesXArray, changedTouchesYArray);
        mRecorder.reset();

    }

    // For single touch
    private void assetTouchInfo(LinkedHashMap<String, String> results,
                                String tag, String type, float[] downX, float[] downY,
                                int touchesLength, int changedTouchesLength) {
        assetTouchInfo(results, tag, type, 0, 0, touchesLength, downX, downY,
                changedTouchesLength, downX, downY);
    }

    // For single touch
    private void assetTouchInfo(LinkedHashMap<String, String> results,
                                String tag, String type, float offsetX, float offsetY,
                                float[] downX, float[] downY,
                                int touchesLength, int changedTouchesLength) {
        assetTouchInfo(results, tag, type, offsetX, offsetY, touchesLength, downX, downY,
                changedTouchesLength, downX, downY);
    }

    // For multiple touch
    private void assetTouchInfo(LinkedHashMap<String, String> results,
                                String tag, String type,
                                int touchesLength, float[] downX, float[] downY,
                                int changedTouchesLength, float[] changedDownX,
                                float[] changedDownY) {
        assetTouchInfo(results, tag, type, 0, 0, touchesLength, downX, downY,
                changedTouchesLength, changedDownX, changedDownY);
    }

    // For multiple touch
    private void assetTouchInfo(LinkedHashMap<String, String> results,
                                String tag, String type, float offsetX, float offsetY,
                                int touchesLength, float[] downX, float[] downY,
                                int changedTouchesLength, float[] changedDownX,
                                float[] changedDownY) {
        assertTrue(results.size() > 0);
        assertEquals(tag, results.get("tag"));
        assertEquals(type, results.get("type"));
        assertEquals(String.valueOf(touchesLength), results.get("touches.length"));
        for (int i = 0; i < touchesLength; ++i) {
            assertTrue((int) (Double.valueOf(results.get("touches[" + i + "].screenX")) - downX[i]) == 0);
            assertTrue((int) (Double.valueOf(results.get("touches[" + i + "].screenY")) - downY[i]) == 0);
            assertTrue((int) (Double.valueOf(results.get("touches[" + i + "].clientX")) - (downX[i] - offsetX)) == 0);
            assertTrue((int) (Double.valueOf(results.get("touches[" + i + "].clientY")) - (downY[i] - offsetY)) == 0);
        }

        assertEquals(String.valueOf(changedTouchesLength), results.get("changedTouches.length"));
        for (int i = 0; i < changedTouchesLength; ++i) {
            assertTrue((int) (Double.valueOf(results.get("changedTouches[" + i + "].screenX")) - changedDownX[i]) == 0);
            assertTrue((int) (Double.valueOf(results.get("changedTouches[" + i + "].screenY")) - changedDownY[i]) == 0);
            assertTrue((int) (Double.valueOf(results.get("changedTouches[" + i + "].clientX")) - (changedDownX[i] - offsetX)) == 0);
            assertTrue((int) (Double.valueOf(results.get("changedTouches[" + i + "].clientY")) - (changedDownY[i] - offsetY)) == 0);
        }
    }

    @Override
    public String getBundleNameForTest() {
        return "TouchInfoTest";
    }

    @Override
    public Map<String, Object> getExposedToJSObjectList() {
        HashMap<String, Object> objects = new HashMap<>();
        mRecorder = new TestResultRecorder();
        objects.put(TestResultRecorder.TAG, mRecorder);
        return objects;
    }
}
