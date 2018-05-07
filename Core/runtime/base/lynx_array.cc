// Copyright 2017 The Lynx Authors. All rights reserved.

#include <sstream>
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

    base::ScopedPtr<LynxValue> LynxArray::Release(int index) {
        LynxValue* temp = values_[index];
        values_[index] = NULL;
        RemovePtr(temp);
        return jscore::LynxValue::MakeValueScoped(temp);
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
            ptrs_.release(value);
        }
    }

    void LynxArray::AddPtr(LynxValue* value) {
        ptrs_.add(value);
    }

    std::string LynxArray::ToString() {
        std::stringstream stream;
        stream<<"[";
        for (int i = 0; i < Size(); ++i) {
            if (Get(i) != NULL) {
                stream << Get(i)->ToString();
            }
            if (i != Size() - 1) {
                stream << ", ";
            }
        }
        stream<<"]";
        return stream.str();
    }
}