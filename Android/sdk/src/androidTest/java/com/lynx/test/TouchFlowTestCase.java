// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.test;

import com.lynx.utils.ScreenUtil;

import java.util.HashMap;
import java.util.LinkedHashMap;
import java.util.Map;

/**
 * 测试事件流流向是否符合预期，包括当具有控制事件流的标志时候
 */
public class TouchFlowTestCase extends BaseTestCase {
    private static final int DELAY_TIME = 200;
    private TestResultRecorder mRecorder;
    private float mRatio;

    @Override
    protected void setUp() throws Exception {
        super.setUp();
        mRatio = (float) ScreenUtil.getScreenWidth() / 750;
    }

    private void addEventListenerForParent(String type) {
        getActivity().getLynxView().loadScriptData("addEventListenerForParent('" + type + "')");
    }

    private void addEventListenerForChild(String type) {
        getActivity().getLynxView().loadScriptData("addEventListenerForChild('" + type + "')");
    }

    private void addEventListener(String parentType, String childType) throws InterruptedException {
        addEventListenerForParent(parentType);
        addEventListenerForChild(childType);
        Thread.sleep(DELAY_TIME);
    }

    // 测试单点触摸在孩子上，同时没有设置任何标志位时的响应顺序
    // 响应顺序： 孩子 -> 父亲
    public void testFlowWithSingleTouchOnChild() throws InterruptedException {
        Thread.sleep(1000);
        addEventListener("touchstart", "touchstart");
        SingleTouchGenerator generator = new SingleTouchGenerator(getActivity());
        generator.down(20 * mRatio, 20 * mRatio);
        Thread.sleep(DELAY_TIME);
        LinkedHashMap<String, String> result = mRecorder.getResults();
        assertTrue(result.size() == 2);
        assertEquals(result.get("child"), "0");
        assertEquals(result.get("parent"), "1");
    }

    // 测试单点触摸在孩子上，孩子设置了 capture 标志位时的响应顺序
    // 响应顺序： 孩子 -> 父亲
    public void testFlowWithSingleTouchOnChildWhenChildHasCaptureMark() throws InterruptedException {
        Thread.sleep(1000);
        addEventListener("touchstart", "touchstart.capture");
        SingleTouchGenerator generator = new SingleTouchGenerator(getActivity());
        generator.down(20 * mRatio, 20 * mRatio);
        Thread.sleep(DELAY_TIME);
        LinkedHashMap<String, String> result = mRecorder.getResults();
        assertTrue(result.size() == 2);
        assertEquals(result.get("child"), "0");
        assertEquals(result.get("parent"), "1");
    }

    // 测试单点触摸在孩子上，父亲设置了 capture 标志位时的响应顺序
    // 响应顺序： 父亲 -> 孩子
    public void testFlowWithSingleTouchOnChildWhenParentHasCaptureMark() throws InterruptedException {
        Thread.sleep(1000);
        addEventListener("touchstart.capture", "touchstart");
        SingleTouchGenerator generator = new SingleTouchGenerator(getActivity());
        generator.down(20 * mRatio, 20 * mRatio);
        Thread.sleep(DELAY_TIME);
        LinkedHashMap<String, String> result = mRecorder.getResults();
        assertTrue(result.size() == 2);
        assertEquals(result.get("child"), "1");
        assertEquals(result.get("parent"), "0");
    }

    // 测试单点触摸在孩子上，父亲设置了 capture 标志位时的响应顺序
    // 响应顺序： 父亲 -> 孩子
    public void testFlowWithSingleTouchOnChildWhenBothHasCaptureMark() throws InterruptedException {
        Thread.sleep(1000);
        addEventListener("touchstart.capture", "touchstart.capture");
        SingleTouchGenerator generator = new SingleTouchGenerator(getActivity());
        generator.down(20 * mRatio, 20 * mRatio);
        Thread.sleep(DELAY_TIME);
        LinkedHashMap<String, String> result = mRecorder.getResults();
        assertTrue(result.size() == 2);
        assertEquals(result.get("child"), "1");
        assertEquals(result.get("parent"), "0");
    }

    // 测试单点触摸在孩子上，父亲设置 stop 标志位时的响应顺序
    // 响应顺序： 孩子 -> 父亲
    public void testFlowWithSingleTouchOnChildWhenParentHasStopMark() throws InterruptedException {
        Thread.sleep(1000);
        addEventListener("touchstart.stop", "touchstart");
        SingleTouchGenerator generator = new SingleTouchGenerator(getActivity());
        generator.down(20 * mRatio, 20 * mRatio);
        Thread.sleep(DELAY_TIME);
        LinkedHashMap<String, String> result = mRecorder.getResults();
        assertTrue(result.size() == 2);
        assertEquals(result.get("child"), "0");
        assertEquals(result.get("parent"), "1");
    }

    // 测试单点触摸在孩子上，孩子设置 stop 标志位时的响应顺序
    // 响应顺序： 孩子（父亲不响应）
    public void testFlowWithSingleTouchOnChildWhenChildHasStopMark() throws InterruptedException {
        Thread.sleep(1000);
        addEventListener("touchstart", "touchstart.stop");
        SingleTouchGenerator generator = new SingleTouchGenerator(getActivity());
        generator.down(20 * mRatio, 20 * mRatio);
        Thread.sleep(DELAY_TIME);
        LinkedHashMap<String, String> result = mRecorder.getResults();
        assertTrue(result.size() == 1);
        assertEquals(result.get("child"), "0");
    }

    // 测试单点触摸在孩子上，孩子和父亲同时设置 self 标志位时的响应顺序
    // 响应顺序： 只有孩子响应
    public void testFlowWithSingleTouchOnChildWhenChildHasSelfMark() throws InterruptedException {
        Thread.sleep(1000);
        addEventListener("touchstart.self", "touchstart.self");
        SingleTouchGenerator generator = new SingleTouchGenerator(getActivity());
        generator.down(20 * mRatio, 20 * mRatio);
        Thread.sleep(DELAY_TIME);
        LinkedHashMap<String, String> result = mRecorder.getResults();
        assertTrue(result.size() == 1);
        assertEquals(result.get("child"), "0");
    }

    // 测试单点触摸在孩子上，父亲设置 capture 和 stop 标志，孩子设置 stop 标志时的响应顺序
    // 响应顺序： 父亲（孩子不响应）
    public void testFlowWithSingleTouchOnChildWhenParentHasStopAndCaptureMarkChildHasStopMark() throws InterruptedException {
        Thread.sleep(1000);
        addEventListener("touchstart.capture.stop", "touchstart.stop");
        SingleTouchGenerator generator = new SingleTouchGenerator(getActivity());
        generator.down(20 * mRatio, 20 * mRatio);
        Thread.sleep(DELAY_TIME);
        LinkedHashMap<String, String> result = mRecorder.getResults();
        assertTrue(result.size() == 1);
        assertEquals(result.get("parent"), "0");
    }

    // 测试多点触摸在孩子上，无标志时的响应顺序
    // 响应顺序：孩子 -> 父亲
    public void testMultiTouchFlowInSameView() throws InterruptedException {
        Thread.sleep(1000);
        addEventListener("touchstart", "touchstart");
        MultiTouchGenerator generator = new MultiTouchGenerator(getActivity());
        generator.down(20 * mRatio, 20 * mRatio);
        Thread.sleep(DELAY_TIME);
        generator.down(30 * mRatio, 30 * mRatio);
        Thread.sleep(DELAY_TIME);
        LinkedHashMap<String, String> result = mRecorder.getResults();
        assertTrue(result.size() == 2);
        assertEquals(result.get("parent"), "3");
        assertEquals(result.get("child"), "2");
    }

    // 测试多点触摸先后分别在孩子和父亲上，无标志时的响应顺序
    // 响应顺序：孩子 -> 父亲
    public void testMultiTouchFlowInDifferentView() throws InterruptedException {
        Thread.sleep(1000);
        addEventListener("touchstart", "touchstart");
        MultiTouchGenerator generator = new MultiTouchGenerator(getActivity());
        generator.down(20 * mRatio, 20 * mRatio);
        Thread.sleep(DELAY_TIME);
        generator.down(80 * mRatio, 80 * mRatio);
        Thread.sleep(DELAY_TIME);
        LinkedHashMap<String, String> result = mRecorder.getResults();
        assertTrue(result.size() == 2);
        assertEquals(result.get("parent"), "3");
        assertEquals(result.get("child"), "2");
    }

    // 单点滑动在 scrollview 的孩子上时，同时孩子具有 prevent 的标志
    // 结果： scrollview 不能滑动
    public void testScrollWhenChildHasPreventMark() throws InterruptedException {
        Thread.sleep(1000);
        SingleTouchGenerator generator = new SingleTouchGenerator(getActivity());
        generator.down(20 * mRatio, 120 * mRatio);
        Thread.sleep(10);
        for (int i = 0; i < 100; i++) {
            generator.move((20 - i) * mRatio, 120 * mRatio);
            Thread.sleep(10);
        }
        Thread.sleep(DELAY_TIME);
        LinkedHashMap<String, String> result = mRecorder.getResults();
        assertTrue(result.size() == 1);
        assertTrue(!result.containsKey("scroll"));
        assertEquals(result.get("child"), "touchmove");
    }

    // 单点触控在 fixed 孩子下，响应的顺序
    // 只有fixed节点响应
    public void testTouchOnFixedChild() throws InterruptedException {
        Thread.sleep(1000);
        addEventListener("touchstart", "touchstart");
        SingleTouchGenerator generator = new SingleTouchGenerator(getActivity());
        generator.down(5 * mRatio, 5 * mRatio);
        Thread.sleep(DELAY_TIME);
        LinkedHashMap<String, String> result = mRecorder.getResults();
        assertTrue(result.size() == 1);
        assertTrue(result.containsKey("fixed"));
    }

    @Override
    public String getBundleNameForTest() {
        return "TouchFlowTest";
    }

    @Override
    public Map<String, Object> getExposedToJSObjectList() {
        HashMap<String, Object> objects = new HashMap<>();
        mRecorder = new TestResultRecorder();
        objects.put(TestResultRecorder.TAG, mRecorder);
        return objects;
    }
}
