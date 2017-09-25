// Copyright 2017 The Lynx Authors. All rights reserved.

#include "runtime/jsc/objects/object_template.h"
#include "runtime/jsc/jsc_helper.h"
#include "runtime/jsc/class_wrap.h"
#include "runtime/jsc/jsc_context.h"

namespace jscore {
    ObjectTemplate::ObjectTemplate(JSCContext* context, LynxObjectTemplate* obj)
            : ObjectWrap(context), target_(obj) {
        target_->set_object_wrap(this);
    }

    ObjectTemplate::~ObjectTemplate() {

    }

    void ObjectTemplate::BindingClass(ClassWrap* class_wrap, LynxObjectTemplate* object) {
        if (object != 0) {

            const std::unordered_map<std::string, LynxObjectTemplate::Field>& field_map
                    = object->fields();
            for (auto& it : field_map) {
                LynxObjectTemplate::Field field = it.second;
                JSObjectGetPropertyCallback get = ObjectTemplate::GetPropertyCallback;
                JSObjectSetPropertyCallback set = ObjectTemplate::SetPropertyCallback;
                if (field.get_callback == 0) get = 0;
                if (field.set_callback == 0) set = 0;
                class_wrap->SetJSStaticValue(it.first.c_str(),
                                             get,
                                             set,
                                             NULL);
            }

            const std::unordered_map<std::string, LynxObjectTemplate::LynxMethodCallback>& method_map
                    = object->methods();
            for (auto& it : method_map) {
                class_wrap->SetJSStaticFunction(it.first.c_str(),
                                                ObjectTemplate::MethodCallback, NULL);
            }
        }
    }

    JSObjectRef ObjectTemplate::Create(JSCContext* context,
                                       ClassWrap* class_wrap,
                                       LynxObjectTemplate* obj) {
        ObjectTemplate* object = lynx_new ObjectTemplate(context, obj);
        JSObjectRef js_object = class_wrap->MakeObject(context->GetContext());
        ObjectWrap::Wrap(object, js_object);
        return js_object;
    }

    JSValueRef ObjectTemplate::GetPropertyCallback(JSContextRef ctx,
                                                   JSObjectRef object,
                                                   JSStringRef propertyName,
                                                   JSValueRef* exception) {
        ObjectTemplate* obj = ObjectWrap::Unwrap<ObjectTemplate>(object);
        std::string property_name = JSCHelper::ConvertToString(ctx, propertyName);
        base::ScopedPtr<LynxValue> value = obj->target()->GetPropertyCallback(property_name);
        return JSCHelper::ConvertToJSValue(ctx, value.Get());
    }

    bool ObjectTemplate::SetPropertyCallback(JSContextRef ctx,
                                             JSObjectRef object,
                                             JSStringRef propertyName,
                                             JSValueRef value,
                                             JSValueRef* exception) {
        ObjectTemplate* obj = ObjectWrap::Unwrap<ObjectTemplate>(object);
        std::string property_name = JSCHelper::ConvertToString(ctx, propertyName);
        base::ScopedPtr<LynxValue> js_value(JSCHelper::ConvertToLynxValue(ctx, value));
        obj->target()->SetPropertyCallback(property_name, js_value);
        return true;
    }

    JSValueRef ObjectTemplate::MethodCallback(JSContextRef ctx,
                                              JSObjectRef function,
                                              JSObjectRef thisObject,
                                              size_t argumentCount,
                                              const JSValueRef arguments[],
                                              JSValueRef* exception) {
        ObjectTemplate* obj = ObjectWrap::Unwrap<ObjectTemplate>(thisObject);

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
        return JSCHelper::ConvertToJSValue(ctx, obj->target()->MethodCallback(function_name, js_value).Get());
    }

}