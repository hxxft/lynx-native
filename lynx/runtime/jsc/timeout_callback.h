// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_RUNTIME_JSC_TIMEOUT_CALLBACK_H_
#define LYNX_RUNTIME_JSC_TIMEOUT_CALLBACK_H_

namespace jscore {
    
    class ClassWrap;
    
    class TimeoutCallback {
    public:
        static void BindingClass(ClassWrap* class_wrap);
        TimeoutCallback() {}
        virtual ~TimeoutCallback() {}
        
    };
}

#endif // LYNX_RUNTIME_JSC_TIMEOUT_CALLBACK_H_
