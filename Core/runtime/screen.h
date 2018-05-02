// Copyright 2017 The Lynx Authors. All rights reserved.
#ifndef LYNX_RUNTIME_SCREEN_H_
#define LYNX_RUNTIME_SCREEN_H_

#include "runtime/base/lynx_object_template.h"

namespace jscore {
    class Screen : public LynxObjectTemplate {
    public:
        Screen();
        virtual ~Screen();
    private:

        static base::ScopedPtr<LynxValue> GetWidthCallback(LynxObjectTemplate* object);
        static base::ScopedPtr<LynxValue> GetHeightCallback(LynxObjectTemplate* object);
    };
}

#endif //LYNX_RUNTIME_SCREEN_H_
