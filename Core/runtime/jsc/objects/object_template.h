// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_RUNTIME_JSC_OBJECTS_OBJECT_TEMPLATE_H_
#define LYNX_RUNTIME_JSC_OBJECTS_OBJECT_TEMPLATE_H_

#include "base/scoped_ptr.h"
#include "runtime/jsc/object_wrap.h"

namespace jscore {
    class JSCContext;
    class LynxObjectTemplate;
    class ClassWrap;

    class ObjectTemplate : public ObjectWrap {
    public:
        virtual ~ObjectTemplate();
        static void BindingClass(ClassWrap* class_wrap, LynxObjectTemplate* object = 0);
        static JSObjectRef Create(JSCContext* context, ClassWrap* class_wrap, LynxObjectTemplate* obj);

        LynxObjectTemplate* target() {
            return target_.Get();
        }

        static JSValueRef GetPropertyCallback(JSContextRef ctx,
                                      JSObjectRef object,
                                      JSStringRef propertyName,
                                      JSValueRef* exception);
        static bool SetPropertyCallback(JSContextRef ctx,
                                JSObjectRef object,
                                JSStringRef propertyName,
                                JSValueRef value,
                                JSValueRef* exception);
        static JSValueRef MethodCallback(JSContextRef ctx,
                                         JSObjectRef function,
                                         JSObjectRef thisObject,
                                         size_t argumentCount,
                                         const JSValueRef arguments[],
                                         JSValueRef* exception);

    private:
        ObjectTemplate(JSCContext* context, LynxObjectTemplate* obj);

        base::ScopedPtr<LynxObjectTemplate> target_;
    };
}


#endif //LYNX_RUNTIME_JSC_OBJECTS_OBJECT_TEMPLATE_H_
