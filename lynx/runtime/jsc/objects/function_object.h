// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef ANDROID_FUNCTION_OBJECT_H
#define ANDROID_FUNCTION_OBJECT_H

#include "base/scoped_ptr.h"
#include "runtime/jsc/object_wrap.h"

namespace jscore{
    class JSCContext;
    class ClassWrap;
    class LynxFunctionObject;

    class FunctionObject : public ObjectWrap {
    public:
        virtual ~FunctionObject();
        static JSObjectRef Create(JSCContext* context, ClassWrap* class_wrap, LynxFunctionObject* obj);

        LynxFunctionObject* target() {
            return target_.Get();
        }

        static JSValueRef MethodCallback(JSContextRef ctx,
                                         JSObjectRef function,
                                         JSObjectRef thisObject,
                                         size_t argumentCount,
                                         const JSValueRef arguments[],
                                         JSValueRef* exception);

    private:
        FunctionObject(JSCContext* context, LynxFunctionObject* obj);

        base::ScopedPtr<LynxFunctionObject> target_;
    };
}


#endif //ANDROID_FUNCTION_OBJECT_H
