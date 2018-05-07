// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_RUNTIME_GLOBAL_H_
#define LYNX_RUNTIME_GLOBAL_H_

#include "runtime/base/lynx_object.h"

namespace jscore {
    class Global : public LynxObject {
    public:
        Global(JSContext* context);
        virtual ~Global();

        base::ScopedPtr<LynxValue> SetTimeout(base::ScopedPtr<LynxArray> &array);

        base::ScopedPtr<LynxValue> SetInterval(base::ScopedPtr<LynxArray> &array);

        base::ScopedPtr<LynxValue> ClearTimeout(base::ScopedPtr<LynxArray> &array);

        base::ScopedPtr<LynxValue> ClearInterval(base::ScopedPtr<LynxArray> &array);

        base::ScopedPtr<LynxValue> GetDevicePixelRatio();

    };
}

#endif //LYNX_RUNTIME_GLOBAL_H_
