// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef RUNTIME_LYNX_FUNTION_OBJECT_H
#define RUNTIME_LYNX_FUNTION_OBJECT_H

#include <string>
#include <unordered_map>
#include "base/scoped_ptr.h"

namespace jscore {
    class LynxValue;
    class LynxArray;
    class LynxObjectTemplate;

    class LynxFunctionObject {
    public:
        typedef base::ScopedPtr<jscore::LynxValue> (*LynxMethodCallback)
                (jscore::LynxFunctionObject*,
                 const std::string& name,
                 base::ScopedPtr<jscore::LynxArray>);

        LynxFunctionObject();
        virtual ~LynxFunctionObject();
        void RegisterMethodCallback(std::string method_name,
                                    LynxMethodCallback callback);

        base::ScopedPtr<LynxValue> MethodCallback(std::string name,
                                                base::ScopedPtr<LynxArray> value);

        inline std::unordered_map<std::string, LynxMethodCallback> methods() {
            return methods_;
        }

    private:

        std::unordered_map<std::string, LynxMethodCallback> methods_;
    };
}


#endif //RUNTIME_LYNX_FUNTION_OBJECT_H
