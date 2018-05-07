// Copyright 2017 The Lynx Authors. All rights reserved.

#include "lynx_map.h"
#include <assert.h>
#include <sstream>

namespace jscore {

    LynxMap::LynxMap() : LynxValue(LynxValue::Type::VALUE_LYNX_MAP) {
        data_.lynx_map = this;
    }

    LynxMap::~LynxMap() {
        data_.lynx_map = NULL;
    }

    void LynxMap::Set(std::string name, LynxValue* value) {
        properties_[name] = value;
        if (value != NULL) {
            AddPtr(value);
        }
    }

    void LynxMap::Delete(std::string name) {
        auto it = properties_.find(name);
        if (it != properties_.end()) {
            RemovePtr(it->second);
            properties_.erase(it);
        }
    }

    LynxValue* LynxMap::GetProperty(std::string name) {
        auto it = properties_.find(name);
        if (it != properties_.end()) {
            return it->second;
        }
        LynxValue* value = 0;
        return value;
    }

    size_t LynxMap::Size() {
        return properties_.size();
    }

    std::string LynxMap::GetName(int index) {
        std::unordered_map<std::string, LynxValue*>::iterator it;
        for (it = properties_.begin(); it != properties_.end() && index > 0; ++it, --index);
        assert(it != properties_.end());
        return it->first;
    }

    void LynxMap::AddPtr(LynxValue* value) {
        ptrs_.add(value);
    }

    void LynxMap::RemovePtr(LynxValue* value) {
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

    std::string LynxMap::ToString() {
        std::stringstream stream;
        int length = Size();
        stream << "{";
        for (int i = 0; i < length; ++i) {
            std::string name = GetName(i);
            stream << name << ": "
                   << (GetProperty(name) != NULL ? GetProperty(name)->ToString() : "");
            if (i != length - 1) {
                stream << ", ";
            }
        }
        stream << "}";
        return stream.str();
    }
}
