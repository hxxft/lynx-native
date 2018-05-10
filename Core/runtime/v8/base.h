// Copyright 2017 The Lynx Authors. All rights reserved.

#if USING_V8
#ifndef RUNTIME_V8_BASE
#define RUNTIME_V8_BASE

#include "v8.h"

namespace jscore {

    typedef v8::FunctionCallback JSMethodCallback;

    typedef v8::AccessorGetterCallback JSGetPropertyCallback;

    typedef v8::AccessorSetterCallback JSSetPropertyCallback;

    struct Field {
        JSSetPropertyCallback set_callback = 0;
        JSGetPropertyCallback get_callback = 0;
    };
}

#endif //RUNTIME_V8_BASE
#endif
