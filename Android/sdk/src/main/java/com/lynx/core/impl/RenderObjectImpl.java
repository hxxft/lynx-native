// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.core.impl;

import com.lynx.base.CalledByNative;
import com.lynx.base.Position;
import com.lynx.base.Size;
import com.lynx.base.Style;
import com.lynx.base.SupposeJSThread;
import com.lynx.core.base.LynxArray;
import com.lynx.core.base.LynxObject;
import com.lynx.ui.LynxUI;
import com.lynx.ui.LynxUIFactory;
import com.lynx.ui.recycler.ILynxUIRecycler;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class RenderObjectImpl  {

    private native void nativeDispatchEvent(long nativePtr, String event, Object[] params);

    private native void nativeUpdateData(long nativePtr, int attr, Object param);

    private native void nativeFreeJavaRef(long nativePtr);

    private native void nativeConstructJavaRef(long mNativePtr, Object object);

    private long mNativePtr;

    private Position mPosition;

    private Size mSize;

    private String mText;

    private int mRenderObjectType;

    private Style mStyle;

    private LynxUI mLynxUI;

    private List<RenderObjectImpl> mChildren;

    private RenderObjectImpl mParent;

    private RenderObjectImpl mRoot;

    private Map<String, String> mAttributes;

    private Map<Integer, Object> mExternalData;

    private List<String> mEvents;

    private RenderObjectImpl(int type, long ptr) {
        this.mRenderObjectType = type;
        this.mNativePtr = ptr;
        this.mAttributes = new HashMap<>();
        this.mPosition = new Position();
        this.mSize = new Size();
        this.mEvents = new ArrayList<>();
        this.mStyle = new Style();
        if (type == LynxUIFactory.UI_TYPE_BODY) {
            mRoot = this;
        }
    }

    public boolean hasUI() {
        return mLynxUI != null;
    }

    public void freeJavaRef(){
        nativeFreeJavaRef(mNativePtr);
        if (mChildren != null && mChildren.size() > 0){
            for(RenderObjectImpl m:mChildren){
                m.freeJavaRef();
            }
        }
    }

    public void constructJavaRef(){
        nativeConstructJavaRef(mNativePtr, this);
        if (mChildren != null && mChildren.size() > 0){
            for(RenderObjectImpl m:mChildren){
                m.constructJavaRef();
            }
        }
    }

    public LynxUI getUI() {
        if(hasUI()) {
            return mLynxUI;
        }
        return null;
    }

    public void setUI(LynxUI impl) {
        mLynxUI = impl;
    }

    @CalledByNative
    public void insertChild(RenderObjectImpl child, int index) {

        if(child == null) return;
        if(mChildren == null) {
            mChildren  = new ArrayList<>();
        }

        child.mParent = this;

        if(index < 0) {
            mChildren.add(child);
        }else{
            mChildren.add(index, child);
        }

        if (mRoot != null) {
            child.setRootRenderObjectImpl(mRoot);
        }

        if (this.getRenderObjectType() == LynxUIFactory.UI_TYPE_LIST_SHADOW
                && mParent != null
                && mParent.hasUI()
                && mParent.getUI() instanceof ILynxUIRecycler) {
            ((ILynxUIRecycler) mParent.getUI()).notifyChildrenChanged();
        }

        if(hasUI()) {
            getUI().insertChild(child, index);
        }
    }

    @CalledByNative
    public void removeChild(RenderObjectImpl child) {

        child.mParent = null;
        mChildren.remove(child);

        if(hasUI()) {
            getUI().removeChild(child);
        }

        if (this.getRenderObjectType() == LynxUIFactory.UI_TYPE_LIST_SHADOW
                && mParent != null
                && mParent.hasUI()
                && mParent.getUI() instanceof ILynxUIRecycler) {
            ((ILynxUIRecycler) mParent.getUI()).notifyChildrenChanged();
        }
    }

    public RenderObjectImpl getChildAt(int index) {
        return mChildren.get(index);
    }

    public int getChildCount() {
        if (mChildren == null) {
            return 0;
        }
        return mChildren.size();
    }

    @CalledByNative
    public void requestLayout() {
        if(hasUI()) {
            getUI().requestLayout();
        }
    }

    @CalledByNative
    public void setText(String text) {
        mText = text;
        if(hasUI()) {
            getUI().setText(text);
        }
    }

    @CalledByNative
    public void setPosition(Position position) {
        mPosition = position;
        if(hasUI()) {
            getUI().setPosition(position);
        }
    }

    @CalledByNative
    public void setSize(Size size) {
        mSize = size;
        if (hasUI()) {
            getUI().setSize(size);
        }
    }
    
    @CalledByNative
    public void updateStyle(Style style) {
        mStyle = style;
        if(hasUI()) {
            getUI().updateStyle(style);
        }
    }

    @CalledByNative
    public void setAttribute(String key, String value) {
        mAttributes.put(key, value);
        if(hasUI()) {
            getUI().setAttribute(key, value);
        }
    }

    public int getRenderObjectType() {
        return mRenderObjectType;
    }

    public Position getPosition() { return mPosition; }

    public Size getSize() { return mSize; }

    public Style getStyle() { return  mStyle; }

    public String getText() { return  mText; }

    public RenderObjectImpl getParent() { return  mParent; }

    @CalledByNative
    public void addEventListener(String event) {
        mEvents.add(event);
        if(hasUI()) {
            getUI().addEventListener(event);
        }
    }

    @CalledByNative
    public void removeEventListener(String event) {
        mEvents.add(event);
        if(hasUI()) {
            getUI().removeEventListener(event);
        }
    }

    // Should be called on JS thread
    @SupposeJSThread
    public void dispatchEvent(String event, Object[] params) {
        nativeDispatchEvent(mNativePtr, event, params);
    }

    // Should be called on JS thread
    @SupposeJSThread
    public void updateData(RenderObjectAttr attr, Object param) {
        nativeUpdateData(mNativePtr, attr.value(), param);
    }

    @CalledByNative
    public void setData(int key, Object data) {
        if (mExternalData == null) {
            mExternalData = new HashMap<>();
        }
        mExternalData.put(key, data);
        if (hasUI()) {
            getUI().setData(key, data);
        }
    }

    public List<String> getEvents() {
        return mEvents;
    }

    public String getAttribute(String key) {
        return mAttributes.get(key);
    }

    public Map<String, String> getAttribution() {
        return mAttributes;
    }

    public Object getData(Integer key) {
        if (mExternalData == null) {
            return null;
        }
        return mExternalData.get(key);
    }

    public void setRootRenderObjectImpl(RenderObjectImpl root) {
        mRoot = root;
        if (mChildren != null) {
            for (RenderObjectImpl child : mChildren) {
                child.setRootRenderObjectImpl(root);
            }
        }
    }

    public RenderObjectImpl getRootRenderObjectImpl() {
        return mRoot;
    }

    @CalledByNative
    public static RenderObjectImpl create(int type, long ptr) {
        return new RenderObjectImpl(type, ptr);
    }

}
