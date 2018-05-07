// Copyright 2017 The Lynx Authors. All rights reserved.

#include "runtime/jsc/jsc_helper.h"

#include <sstream>
#include <string>
#include "base/debug/memory_debug.h"
#include "base/debug/timing_tracker.h"
#include "runtime/global.h"
#include "runtime/base/lynx_value.h"
#include "runtime/base/lynx_object.h"
#include "runtime/base/lynx_array.h"
#include "runtime/base/lynx_map.h"
#include "runtime/base/lynx_holder.h"
#include "runtime/js/class_template_storage.h"
#include "runtime/jsc/jsc_context.h"
#include "runtime/jsc/js_value_ext.h"
#include "runtime/jsc/jsc_function.h"
#include "runtime/jsc/prototype_builder.h"
#include "runtime/jsc/object_wrap.h"

namespace jscore {

    JSValueRef JSCHelper::ConvertToJSString(JSContextRef ctx, const std::string &s) {
        JSStringRef ref = JSStringCreateWithUTF8CString(s.c_str());
        JSValueRef result = JSValueMakeString(ctx, ref);
        JSStringRelease(ref);
        return result;
    }

    JSValueRef JSCHelper::ConvertToJSString(JSContextRef ctx, const char* s) {
        JSStringRef ref = JSStringCreateWithUTF8CString(s);
        JSValueRef result = JSValueMakeString(ctx, ref);
        JSStringRelease(ref);
        return result;
    }

    std::string JSCHelper::ConvertToString(JSContextRef ctx, JSStringRef value) {
        std::string str;
        size_t len = JSStringGetMaximumUTF8CStringSize(value);
        base::ScopedPtr<char[]> ch(lynx_new char[len]);
        JSStringGetUTF8CString(value, ch.Get(), len);
        str = ch.Get();
        return str;
    }

    std::string JSCHelper::ConvertToString(JSContextRef ctx, JSValueRef value) {
        if(JSValueIsString(ctx, value) || JSValueIsObject(ctx, value)) {
            JSStringRef js_str = JSValueToStringCopy(ctx, value, NULL);
            size_t len = JSStringGetMaximumUTF8CStringSize(js_str);
            base::ScopedPtr<char[]> ch(lynx_new char[len]);
            size_t size = JSStringGetUTF8CString(js_str, ch.Get(), len);
            JSStringRelease(js_str);
            return std::string(ch.Get(), size - 1);
        }
        return "";
    }

    base::ScopedPtr<LynxValue> JSCHelper::ConvertToLynxValue(JSContextRef ctx, JSValueRef value) {
        base::ScopedPtr<LynxValue> js_value;
        if (JSValueIsNull(ctx, value)) {
            js_value = LynxValue::MakeNull();
        } else if (JSValueIsUndefined(ctx, value)) {
            js_value = LynxValue::MakeUndefined();
        } else if (JSValueIsBoolean(ctx, value)) {
            js_value = LynxValue::MakeBool(JSValueToBoolean(ctx, value));
        } else if (JSValueIsNumber(ctx, value)) {
            double number = JSValueToNumber(ctx, value, NULL);
            js_value = (number - (int) number) == 0 ?
                       LynxValue::MakeInt(number) : LynxValue::MakeDouble(number);
        } else if (JSValueIsString(ctx, value)) {
            js_value = LynxValue::MakeString(ConvertToString(ctx, value));
        } else if (JSValueIsArrayExt(ctx, value)) {
            js_value = base::ScopedPtr<LynxValue>(
                    ConvertToLynxArray(ctx, (JSObjectRef) value).Release());
        } else if (JSValueIsObject(ctx, value)) {
            if (JSObjectIsFunction(ctx, (JSObjectRef) value)) {
                js_value = ConvertToLynxFunction(ctx, (JSObjectRef) value);
            } else if (JSObjectGetPrivate((JSObjectRef) value) != NULL) {
                js_value = LynxValue::MakeObject(
                        ConvertToLynxObject(ctx, (JSObjectRef) value));
            } else {
                js_value = base::ScopedPtr<LynxValue>(
                        ConvertToLynxMap(ctx, (JSObjectRef) value).Release());
            }
        }
        return js_value;
    }

    base::ScopedPtr<LynxArray> JSCHelper::ConvertToLynxArray(JSContextRef ctx, JSObjectRef value) {
        base::ScopedPtr<LynxArray> array(lynx_new LynxArray());
        JSStringRef name = JSStringCreateWithUTF8CString("length");
        int length = JSValueToNumber(ctx, JSObjectGetProperty(ctx, value, name, NULL), NULL);
        JSStringRelease(name);
        for (int i = 0; i < length; ++i) {
            JSValueRef temp = JSObjectGetPropertyAtIndex(ctx, value, i, NULL);
            array->Push(ConvertToLynxValue(ctx, temp).Release());
        }
        return array;
    }

    base::ScopedPtr<LynxArray> JSCHelper::ConvertToLynxArray(JSContextRef ctx, JSValueRef *value,
                                                             size_t length) {
        base::ScopedPtr<LynxArray> array (lynx_new LynxArray());
        for (int i = 0; i < length; ++i) {
            array->Push(ConvertToLynxValue(ctx, value[i]).Release());
        }
        return array;
    }

    base::ScopedPtr<LynxMap> JSCHelper::ConvertToLynxMap(JSContextRef ctx, JSObjectRef value) {
        base::ScopedPtr<LynxMap> lynx_object(lynx_new LynxMap());
        JSObjectRef obj = JSValueToObject(ctx, value, NULL);
        JSPropertyNameArrayRef names = JSObjectCopyPropertyNames(ctx, obj);
        size_t len = JSPropertyNameArrayGetCount(names);
        for(int i = 0; i < len; ++i) {
            JSStringRef key = JSPropertyNameArrayGetNameAtIndex(names, i);
            std::string key_str = JSCHelper::ConvertToString(ctx, key);
            JSValueRef value = JSObjectGetProperty(ctx, obj, key, NULL);
            LynxValue* lynx_value = ConvertToLynxValue(ctx, value).Release();
            lynx_object->Set(key_str, lynx_value);
        }
        JSPropertyNameArrayRelease(names);
        return lynx_object;
    }

    LynxObject* JSCHelper::ConvertToLynxObject(JSContextRef ctx, JSObjectRef value) {
        return JSCObjectWrap::Unwrap<LynxObject>(value);
    }

    base::ScopedPtr<LynxValue> JSCHelper::ConvertToLynxFunction(JSContextRef ctx, JSObjectRef value) {
        Global* global = JSCObjectWrap::Unwrap<Global>(JSContextGetGlobalObject(ctx));
        JSCContext* context = static_cast<JSCContext*>(global->context());
        JSCFunction* function = lynx_new JSCFunction(context, value);
        return LynxValue::MakeLynxFunction(function);
    }

    JSValueRef JSCHelper::ConvertToJSValue(JSContextRef ctx, LynxValue* value) {
        if (value == 0) {
            return JSValueMakeNull(ctx);
        }
        JSValueRef js_obj = NULL;
        switch (value->type_) {
            case LynxValue::Type::VALUE_INT:
                js_obj = ConvertToJSInt(ctx, value);
                break;
            case LynxValue::Type::VALUE_LONG:
                js_obj = ConvertToJSLong(ctx, value);
                break;
            case LynxValue::Type::VALUE_FLOAT:
                js_obj = ConvertToJSFloat(ctx, value);
                break;
            case LynxValue::Type::VALUE_DOUBLE:
                js_obj = ConvertToJSDouble(ctx, value);
                break;
            case LynxValue::Type::VALUE_BOOL:
                js_obj = ConvertToJSBoolean(ctx, value);
                break;
            case LynxValue::Type::VALUE_STRING:
                js_obj = ConvertToJSString(ctx, value);
                break;
            case LynxValue::Type::VALUE_LYNX_ARRAY:
                js_obj = ConvertToJSArray(ctx, value->data_.lynx_array);
                break;
            case LynxValue::Type::VALUE_LYNX_MAP:
                js_obj = ConvertToJSObject(ctx, value->data_.lynx_map);
                break;
            case LynxValue::Type::VALUE_LYNX_OBJECT:
                js_obj = ConvertToJSObject(ctx, value->data_.lynx_object);
                break;
            case LynxValue::Type::VALUE_LYNX_HOLDER:
                js_obj = ConvertToJSValue(ctx, value->data_.lynx_holder->GetLynxValue().Get());
                break;
            case LynxValue::Type::VALUE_NULL:
                js_obj = JSValueMakeNull(ctx);
                break;
            case LynxValue::Type::VALUE_UNDEFINED:
            default:
                js_obj = JSValueMakeUndefined(ctx);
                break;
        }
        return js_obj;
    }

    JSValueRef* JSCHelper::ConvertToJSValueRefArray(JSContextRef ctx, LynxArray* args) {
        int length = args->Size();
        int i = 0;
        JSValueRef* values = lynx_new JSValueRef[length];
        for (; i < length; ++i) {
            LynxValue* arg = args->Get(i);
            JSValueRef object = ConvertToJSValue(ctx, arg);
            values[i] = object;
        }
        return values;
    }

    JSObjectRef JSCHelper::ConvertToJSArray(JSContextRef ctx, LynxArray* args) {
        base::ScopedPtr<JSValueRef[]> array(ConvertToJSValueRefArray(ctx, args));
        JSObjectRef js_obj = JSObjectMakeArray(ctx, args->Size(), array.Get(), NULL);
        return js_obj;
    }

    JSObjectRef JSCHelper::ConvertToJSObject(JSContextRef ctx, LynxMap* object) {
        if (object == NULL) return NULL;
        JSObjectRef v8_obj = JSObjectMake(ctx, NULL, NULL);
        for (int i = 0; i < object->Size(); ++i) {
            std::string name = object->GetName(i);
            JSStringRef str_ref = JSStringCreateWithUTF8CString(name.c_str());
            JSValueRef property = ConvertToJSValue(ctx, object->GetProperty(name));
            JSObjectSetProperty(ctx, v8_obj, str_ref, property, kJSClassAttributeNone, NULL);
            JSStringRelease(str_ref);
        }
        return v8_obj;
    }

    JSObjectRef JSCHelper::ConvertToJSObject(JSContextRef ctx, LynxObject *object) {
        if (object->object_wrap() != NULL) {
            return static_cast<JSCObjectWrap*>(object->object_wrap())->js_ref();
        }
        Global* global = JSCObjectWrap::Unwrap<Global>(JSContextGetGlobalObject(ctx));
        JSCContext* context = static_cast<JSCContext*>(global->context());
        auto prototype_builder = static_cast<JSCPrototypeBuilder*>(object->class_template()->prototype_builder());
        JSObjectRef js_object = prototype_builder->MakeObject(context->GetContext());
        JSCObjectWrap::Wrap(context, object, js_object);

        return js_object;
    }

}
