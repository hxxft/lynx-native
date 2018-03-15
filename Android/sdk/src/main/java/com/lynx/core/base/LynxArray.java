// Copyright 2017 The Lynx Authors. All rights reserved.
package com.lynx.core.base;

import java.util.ArrayList;
import java.util.List;

public class LynxArray {
    private List<Object> mObjects;

    public LynxArray(int length) {
        mObjects = new ArrayList<>();
        for (int i = 0; i < length; i++) {
            mObjects.add(null);
        }
    }

    public LynxArray() {
        mObjects = new ArrayList<>();
    }

    public void set(int index, Object object) {
        mObjects.set(index, object);
    }

    public void add(Object object) {
        mObjects.add(object);
    }

    public int length() {
        return mObjects.size();
    }

    public Object get(int index) {
        return mObjects.get(index);
    }

    public String getComponentsType() {
        return ParamsTransform.transform(mObjects.toArray());
    }

    public void addAll(LynxArray newData){
        mObjects.addAll(newData.getList());
    }

    private List<Object> getList(){
        return mObjects;
    }

}
