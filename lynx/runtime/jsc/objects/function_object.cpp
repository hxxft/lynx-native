// Copyright 2017 The Lynx Authors. All rights reserved.

#include "runtime/jsc/objects/function_object.h"
#include "runtime/jsc/jsc_class_wrap_storage.h"

#include "base/debug/memory_debug.h"
#include "runtime/jsc/jsc_context.h"
#include "runtime/jsc/jsc_helper.h"
#include "runtime/base/lynx_function_object.h"

namespace jscore {

    FunctionObject::FunctionObject(JSCContext* context, LynxFunctionObject* object)
            : ObjectWrap(context), target_(object){

    }

    FunctionObject::~FunctionObject() {

    }

    JSObjectRef FunctionObject::Create(JSCContext* context,
                                       ClassWrap* class_wrap,
                                       LynxFunctionObject* obj) {
        JSObjectRef js_function_object = class_wrap->MakeObject(context->GetContext());
        std::unordered_map<std::string, LynxFunctionObject::LynxMethodCallback> method_map
                = obj->methods();
        FunctionObject *function_object = lynx_new FunctionObject(context, obj);
        for (auto it = method_map.begin(); it != method_map.end(); ++it) {
            std::string name = it->first;
            JSCHelper::SetFunctionProperty(context->GetContext(),
                                           js_function_object,
                                           name,
                                           FunctionObject::MethodCallback,
                                           kJSPropertyAttributeNone,
                                           NULL);
        }
        ObjectWrap::Wrap(function_object, js_function_object);
        return js_function_object;
    }

    JSValueRef FunctionObject::MethodCallback(JSContextRef ctx,
                                              JSObjectRef function,
                                              JSObjectRef thisObject,
                                              size_t argumentCount,
                                              const JSValueRef arguments[],
                                              JSValueRef *exception) {
        FunctionObject* obj = ObjectWrap::Unwrap<FunctionObject>(thisObject);

        JSStringRef name_key = JSStringCreateWithUTF8CString("name");
        std::string function_name = JSCHelper::ConvertToString(ctx,
                                                               JSObjectGetProperty(ctx,
                                                                                   function,
                                                                                   name_key,
                                                                                   NULL));
        JSStringRelease(name_key);
        base::ScopedPtr<LynxArray> js_value(
                JSCHelper::ConvertToLynxArray(ctx, const_cast<JSValueRef *>(arguments),
                                              argumentCount));
        return JSCHelper::ConvertToJSValue(ctx,
                                           obj->target()->MethodCallback(function_name, js_value).Get());
    }

}