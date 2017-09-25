// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_RUNTIME_BODY_H_
#define LYNX_RUNTIME_BODY_H_

#include "runtime/element.h"

namespace jscore {
    class Body : public Element {
    public:
        Body(JSContext* context);
        virtual ~Body();

    private:
        static base::ScopedPtr<LynxValue> GetClientWidthCallback(LynxObjectTemplate* object);
        static base::ScopedPtr<LynxValue> GetClientHeightCallback(LynxObjectTemplate* object);

    };
}

#endif //LYNX_RUNTIME_BODY_H_
