// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_RUNTIME_BASE_LYNX_MAP_H_
#define LYNX_RUNTIME_BASE_LYNX_MAP_H_

#include <unordered_map>
#include <string>
#include "base/scoped_set.h"
#include "runtime/base/lynx_value.h"

namespace jscore {

    class LynxMap : public LynxValue {
    public:
        LynxMap();
        virtual ~LynxMap();
        void Set(std::string name, LynxValue* value);
        void Delete(std::string name);
        size_t Size();
        std::string GetName(int index);
        LynxValue* GetProperty(std::string name);
        virtual std::string ToString();

    private:
        void RemovePtr(LynxValue* value);
        void AddPtr(LynxValue* value);
        std::unordered_map<std::string, LynxValue*> properties_;
        base::ScopedSet<LynxValue> ptrs_;
    };

}

#endif //LYNX_RUNTIME_BASE_LYNX_MAP_H_
