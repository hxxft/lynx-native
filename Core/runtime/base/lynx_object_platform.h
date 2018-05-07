// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_RUNTIME_LYNX_OBJECT_PLATFORM_H_
#define LYNX_RUNTIME_LYNX_OBJECT_PLATFORM_H_

#include "runtime/base/lynx_object.h"

namespace jscore {

    class LynxObjectPlatform : public LynxObject {
    public:
        LynxObjectPlatform() : LynxObject(NULL, NULL) {}
        virtual ~LynxObjectPlatform(){}

        virtual void Attach(JSContext *context);
    };
}

#endif //LYNX_RUNTIME_LYNX_OBJECT_PLATFORM_H_
