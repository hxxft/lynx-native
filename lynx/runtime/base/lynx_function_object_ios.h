// Copyright 2017 The Lynx Authors. All rights reserved.
#ifndef LYNX_RUNTIME_IOS_LYNX_FUNCTION_OBJECT_H_
#define LYNX_RUNTIME_IOS_LYNX_FUNCTION_OBJECT_H_

#include "runtime/base/lynx_function_object.h"
#include "base/lynx_function_object.h"

namespace jscore {
    class LynxFunctionObjectIOS : public LynxFunctionObject {
    public:
        LynxFunctionObjectIOS(LYXFunctionObject *ios_object);
        virtual ~LynxFunctionObjectIOS();
        
    private:
        static base::ScopedPtr<LynxValue> ExecCallback(LynxFunctionObject* object,
                                                       const std::string& name,
                                                       base::ScopedPtr<LynxArray> array);
        
        LYXFunctionObject *ios_object_;
    };
}


#endif //LYNX_RUNTIME_ANDROID_LYNX_FUNCTION_OBJECT_H_
