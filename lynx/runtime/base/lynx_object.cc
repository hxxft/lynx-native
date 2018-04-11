// Copyright 2017 The Lynx Authors. All rights reserved.

#include "lynx_object.h"
#include <assert.h>

namespace jscore {

    LynxObject::LynxObject() : LynxValue(LynxValue::Type::VALUE_LYNX_OBJECT) {
        data_.lynx_object = this;
    }

    LynxObject::~LynxObject() {
        data_.lynx_object = NULL;
    }

    void LynxObject::Set(std::string name, LynxValue* value) {
        properties_[name] = value;
        if (value != NULL) {
            AddPtr(value);
        }
    }

    void LynxObject::Delete(std::string name) {
        auto it = properties_.find(name);
        if (it != properties_.end()) {
            RemovePtr(it->second);
            properties_.erase(it);
        }
    }

    LynxValue* LynxObject::GetProperty(std::string name) {
        auto it = properties_.find(name);
        if (it != properties_.end()) {
            return it->second;
        }
        LynxValue* value = 0;
        return value;
    }

    size_t LynxObject::Size() {
        return properties_.size();
    }

    std::string LynxObject::GetName(int index) {
        std::unordered_map<std::string, LynxValue*>::iterator it;
        for (it = properties_.begin(); it != properties_.end() && index > 0; ++it, --index);
        assert(it != properties_.end());
        return it->first;
    }

    void LynxObject::AddPtr(LynxValue* value) {
        ptrs_.add(value);
    }

    void LynxObject::RemovePtr(LynxValue* value) {
        bool exist = false;
        for (auto it = properties_.begin(); it != properties_.end(); ++it) {
            if (it->second == value) {
                exist = true;
                break;
            }
        }
        if (!exist) {
            ptrs_.erase(value);
        }
    }
}
