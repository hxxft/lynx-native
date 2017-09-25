// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_RUNTIME_CONSOLE_H_
#define LYNX_RUNTIME_CONSOLE_H_

#include "runtime/base/lynx_object_template.h"

namespace jscore {
    class Console : public LynxObjectTemplate {
    public:
        Console();
        virtual ~Console();

    private:
        static base::ScopedPtr<LynxValue>
        LogCallback(LynxObjectTemplate* object, base::ScopedPtr<LynxArray> array);
    };

}

#endif //LYNX_RUNTIME_CONSOLE_H_
