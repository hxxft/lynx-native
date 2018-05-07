// Copyright 2017 The Lynx Authors. All rights reserved.
#ifndef LYNX_RUNTIME_SCREEN_H_
#define LYNX_RUNTIME_SCREEN_H_

#include "runtime/base/lynx_object.h"

namespace jscore {
    class Screen : public LynxObject {
    public:
        Screen(JSContext* context);
        virtual ~Screen();

        base::ScopedPtr<LynxValue> GetWidth();
        base::ScopedPtr<LynxValue> GetHeight();
    };
}

#endif //LYNX_RUNTIME_SCREEN_H_
