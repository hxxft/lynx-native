// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_RUNTIME_JSC_OBJECTS_WINDOW_OBJECT_H_
#define LYNX_RUNTIME_JSC_OBJECTS_WINDOW_OBJECT_H_

#include <JavaScriptCore/JavaScript.h>

namespace jscore {
    
    class ClassWrap;
    
    class WindowObject {
        public:
        static void BindingClass(ClassWrap* class_wrap);
    };
}

#endif  // LYNX_RUNTIME_JSC_OBJECTS_WINDOW_OBJECT_H_

