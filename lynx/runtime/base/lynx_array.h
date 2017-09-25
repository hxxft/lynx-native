// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_RUNTIME_LYNX_ARRAY_H_
#define LYNX_RUNTIME_LYNX_ARRAY_H_

#include <vector>
#include "runtime/base/lynx_value.h"
#include "base/scoped_set.h"

namespace jscore {

    class LynxArray : public LynxValue {
    public:
        LynxArray();
        virtual ~LynxArray();

        void Push(LynxValue* value);
        LynxValue* Pop();
        LynxValue* Get(int index);
        int Size();

        void Release() {
            ptrs_.release();
        }

    private:
        void RemovePtr(LynxValue* value);
        void AddPtr(LynxValue* value);
        std::vector<LynxValue*> values_;
        base::ScopedSet<LynxValue> ptrs_;
    };
}


#endif //LYNX_RUNTIME_LYNX_ARRAY_H_
