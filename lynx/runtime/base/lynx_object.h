// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_RUNTIME_LYNX_OBJECT_H_
#define LYNX_RUNTIME_LYNX_OBJECT_H_

#include <unordered_map>
#include "base/scoped_set.h"
#include <string>
#include "runtime/base/lynx_value.h"

namespace jscore {

    class LynxObject : public LynxValue {
    public:
        LynxObject();
        virtual ~LynxObject();
        void Set(std::string name, LynxValue* value);
        void Delete(std::string name);
        int Size();
        std::string GetName(int index);
        LynxValue* GetProperty(std::string name);

    private:
        void RemovePtr(LynxValue* value);
        void AddPtr(LynxValue* value);
        std::unordered_map<std::string, LynxValue*> properties_;
        base::ScopedSet<LynxValue> ptrs_;
    };

}

#endif //LYNX_RUNTIME_LYNX_OBJECT_H_
