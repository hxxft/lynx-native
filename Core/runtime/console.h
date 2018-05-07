// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_RUNTIME_CONSOLE_H_
#define LYNX_RUNTIME_CONSOLE_H_

#include "runtime/base/lynx_object.h"

namespace jscore {
    class Console : public LynxObject {
    public:
        Console(JSContext* context);
        virtual ~Console();

        base::ScopedPtr<LynxValue> Log(base::ScopedPtr<LynxArray>& array);
        base::ScopedPtr<LynxValue> Warn(base::ScopedPtr<LynxArray>& array);
        base::ScopedPtr<LynxValue> Error(base::ScopedPtr<LynxArray>& array);
    };

}

#endif //LYNX_RUNTIME_CONSOLE_H_
