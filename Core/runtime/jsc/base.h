// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef RUNTIME_JSC_BASE
#define RUNTIME_JSC_BASE

#include <JavaScriptCore/JavaScript.h>

namespace jscore {

    typedef JSObjectCallAsFunctionCallback JSMethodCallback;

    typedef JSObjectGetPropertyCallback JSGetPropertyCallback;

    typedef JSObjectSetPropertyCallback JSSetPropertyCallback;

    struct Field {
        JSSetPropertyCallback set_callback = 0;
        JSGetPropertyCallback get_callback = 0;
    };
}

#endif //RUNTIME_JSC_BASE
