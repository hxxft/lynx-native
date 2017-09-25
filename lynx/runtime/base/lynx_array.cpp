// Copyright 2017 The Lynx Authors. All rights reserved.

#include "lynx_array.h"

namespace jscore {
    LynxArray::LynxArray() : LynxValue(LynxValue::Type::VALUE_LYNX_ARRAY) {
        data_.lynx_array = this;
    }

    LynxArray::~LynxArray() {
        data_.lynx_array = 0;
    }

    void LynxArray::Push(LynxValue* value) {
        values_.push_back(value);
        if(value != NULL) {
            AddPtr(value);
        }
    }

    LynxValue* LynxArray::Pop() {
        if (values_.size() > 0) {
            LynxValue* value = values_[values_.size() - 1];
            values_.pop_back();
            RemovePtr(value);
            return value;
        }
        return 0;
    }

    LynxValue* LynxArray::Get(int index) {
        return values_[index];
    }

    int LynxArray::Size() {
        return values_.size();
    }

    void LynxArray::RemovePtr(LynxValue* value) {
        bool exist = false;
        for (auto it = values_.begin(); it != values_.end(); ++it) {
            if (*it == value) {
                exist = true;
                break;
            }
        }
        if (!exist) {
            ptrs_.erase(value);
        }
    }

    void LynxArray::AddPtr(LynxValue* value) {
        ptrs_.add(value);
    }

}