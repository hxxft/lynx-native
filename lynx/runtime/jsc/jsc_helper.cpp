// Copyright 2017 The Lynx Authors. All rights reserved.

#include <sstream>
#include <runtime/base/lynx_value.h>
#include <string>
#include <base/debug/timing_tracker.h>
#include "runtime/base/lynx_value.h"
#include "runtime/jsc/objects/object_template.h"
#include "runtime/jsc/objects/function_object.h"
#include "runtime/base/lynx_function_object.h"
#include "jsc_helper.h"
#include "base/debug/memory_debug.h"

#include "runtime/jsc/jsc_context.h"
#include "runtime/jsc/jsc_function.h"
#include "runtime/jsc/jsc_class_wrap_storage.h"

#if OS_IOS
#include "base/ios/common.h"
#endif

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

    std::string JSCHelper::ConvertToString(LynxValue* value) {
        std::string result = "";
        if (value == NULL) {
            return result;
        }
        switch (value->type_) {
            case LynxValue::Type::VALUE_INT: {
                double number = value->data_.i;
                std::stringstream stream;
                stream << number;
                result = stream.str();
            }
                break;
            case LynxValue::Type::VALUE_DOUBLE: {
                double number = value->data_.d;
                std::stringstream stream;
                stream << number;
                result = stream.str();
            }
                break;
            case LynxValue::Type::VALUE_BOOL: {
                if (value->data_.b) {
                    result = "true";
                } else {
                    result = "false";
                }
            }
                break;
            case LynxValue::Type::VALUE_STRING:
                result = value->data_.str;
                break;
            case LynxValue::Type::VALUE_LYNX_ARRAY: {
                std::stringstream stream;
                int length = value->data_.lynx_array->Size();
                stream<<"[";
                for (int i = 0; i < length; ++i) {
                    stream << ConvertToString(value->data_.lynx_array->Get(i));
                    if (i != length - 1) {
                        stream << ", ";
                    }
                }
                stream<<"]";
                result = stream.str();
            }
                break;
            case LynxValue::Type::VALUE_LYNX_OBJECT: {
                    std::stringstream stream;
                    int length = value->data_.lynx_object->Size();
                    stream << "{";
                    for (int i = 0; i < length; ++i) {
                        std::string name = value->data_.lynx_object->GetName(i);
                        stream << name << ": "
                        << ConvertToString(value->data_.lynx_object->GetProperty(name));
                        if (i != length - 1) {
                            stream << ", ";
                        }
                    }
                    stream << "}";
                    result = stream.str();
                }
                break;
            case LynxValue::Type::VALUE_LYNX_FUNCTION_OBJECT:
                result = "function";
                break;
            case LynxValue::Type::VALUE_LYNX_OBJECT_TEMPLATE:
                break;
            case LynxValue::Type::VALUE_NULL:
                result = "null";
                break;
            default:
                break;
        }
        return result;
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
        if (JSValueIsBoolean(ctx, value)) {
            js_value = LynxValue::MakeBool(JSValueToBoolean(ctx, value));
        } else if (JSValueIsNumber(ctx, value)) {
            double number = JSValueToNumber(ctx, value, NULL);
            js_value = (number - (int) number) == 0 ?
                       LynxValue::MakeInt(number) : LynxValue::MakeDouble(number);
        } else if (JSValueIsString(ctx, value)) {
            js_value = LynxValue::MakeString(ConvertToString(ctx, value));
#if OS_IOS // Makes compatible with iOS 8.0
        } else if (!iOS9Later) {
            if (JSValueIsObject(ctx, value)) {
                if (JSObjectIsFunction(ctx, (JSObjectRef) value)) {
                    js_value = ConvertToLynxFunction(ctx, (JSObjectRef) value);
                } else if (JSObjectGetPrivate((JSObjectRef) value) != NULL) {
                    js_value = LynxValue::MakeObjectTemplate(ConvertToLynxObjectTemplate(ctx, (JSObjectRef) value));
                } else if (!JSValueIsUndefined(ctx, GetProperty(ctx, (JSObjectRef) value, "length", 0))) {
                    js_value = ConvertToLynxArray(ctx, (JSObjectRef) value);
                } else {
                    js_value = ConvertToLynxObject(ctx, (JSObjectRef) value);
                }
            }
#endif
        } else if (JSValueIsArray(ctx, value)) {
            js_value = base::ScopedPtr<LynxValue>(
                    ConvertToLynxArray(ctx, (JSObjectRef) value).Release());
        } else if (JSValueIsObject(ctx, value)) {
            if (JSObjectIsFunction(ctx, (JSObjectRef) value)) {
                js_value = ConvertToLynxFunction(ctx, (JSObjectRef) value);
            } else if (JSObjectGetPrivate((JSObjectRef) value) != NULL) {
                js_value = LynxValue::MakeObjectTemplate(
                        ConvertToLynxObjectTemplate(ctx, (JSObjectRef) value));
            } else {
                js_value = base::ScopedPtr<LynxValue>(
                        ConvertToLynxObject(ctx, (JSObjectRef) value).Release());
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

    base::ScopedPtr<LynxArray> JSCHelper::ConvertToLynxArray(JSContextRef ctx, JSValueRef *value, int length) {
        base::ScopedPtr<LynxArray> array (lynx_new LynxArray());
        for (int i = 0; i < length; ++i) {
            array->Push(ConvertToLynxValue(ctx, value[i]).Release());
        }
        return array;
    }

    base::ScopedPtr<LynxObject> JSCHelper::ConvertToLynxObject(JSContextRef ctx, JSObjectRef value) {
        base::ScopedPtr<LynxObject> lynx_object(lynx_new LynxObject());
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

    LynxObjectTemplate* JSCHelper::ConvertToLynxObjectTemplate(JSContextRef ctx, JSObjectRef value) {
        return ObjectWrap::Unwrap<ObjectTemplate>(value)->target();
    }

    base::ScopedPtr<LynxValue> JSCHelper::ConvertToLynxFunction(JSContextRef ctx, JSObjectRef value) {
        JSCContext* context = static_cast<JSCContext*>(JSObjectGetPrivate(JSContextGetGlobalObject(ctx)));
        JSCFunction* function = lynx_new JSCFunction(context, value);
        return LynxValue::MakeLynxFunction(function);
    }

    JSValueRef JSCHelper::ConvertToJSValue(JSContextRef ctx, jscore::LynxValue* value) {
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
            case LynxValue::Type::VALUE_LYNX_OBJECT:
                js_obj = ConvertToJSObject(ctx, value->data_.lynx_object);
                break;
            case LynxValue::Type::VALUE_LYNX_FUNCTION_OBJECT:
                js_obj = ConvertToJSFunctionObject(ctx, value->data_.lynx_function_object);
                break;
            case LynxValue::Type::VALUE_LYNX_OBJECT_TEMPLATE:
                js_obj = ConvertToJSObject(ctx, value->data_.lynx_object_template);
                break;
            case LynxValue::Type::VALUE_NULL:
                js_obj = JSValueMakeNull(ctx);
                break;
            default:
                js_obj = JSValueMakeUndefined(ctx);
                break;
        }
        return js_obj;
    }

    JSValueRef* JSCHelper::ConvertToJSValueRefArray(JSContextRef ctx, jscore::LynxArray* args) {
        int length = args->Size();
        int i = 0;
        JSValueRef* values = lynx_new JSValueRef[length];
        for (; i < length; ++i) {
            jscore::LynxValue* arg = args->Get(i);
            JSValueRef object = ConvertToJSValue(ctx, arg);
            values[i] = object;
        }
        return values;
    }

    JSObjectRef JSCHelper::ConvertToJSArray(JSContextRef ctx, jscore::LynxArray* args) {
        base::ScopedPtr<JSValueRef[]> array(ConvertToJSValueRefArray(ctx, args));
        JSObjectRef js_obj = JSObjectMakeArray(ctx, args->Size(), array.Get(), NULL);
        return js_obj;
    }

    JSObjectRef JSCHelper::ConvertToJSObject(JSContextRef ctx, LynxObject* object) {
        JSObjectRef v8_obj = JSObjectMake(ctx, NULL, NULL);
        if (object != NULL) {
            for (int i = 0; i < object->Size(); ++i) {
                std::string name = object->GetName(i);
                JSStringRef str_ref = JSStringCreateWithUTF8CString(name.c_str());
                JSValueRef property = ConvertToJSValue(ctx, object->GetProperty(name));
                JSObjectSetProperty(ctx, v8_obj, str_ref, property, kJSClassAttributeNone, NULL);
                JSStringRelease(str_ref);
            }
        }
        return v8_obj;
    }

    JSObjectRef JSCHelper::ConvertToJSObject(JSContextRef ctx, LynxObjectTemplate *object) {
        if (object->object_wrap() != NULL) {
            return object->object_wrap()->js_ref();
        }
        JSCContext* context = static_cast<JSCContext*>(JSObjectGetPrivate(JSContextGetGlobalObject(ctx)));
        ClassWrap* class_wrap = context->context_storage()->GetClassWrap(object->class_name());
        if (class_wrap == 0) {
            class_wrap = context->context_storage()->CreateClassWrap(object->class_name());
            ObjectTemplate::BindingClass(class_wrap, object);
        }
        JSObjectRef js_obj = ObjectTemplate::Create(context, class_wrap, object);
        return js_obj;
    }

    JSObjectRef JSCHelper::ConvertToJSFunctionObject(JSContextRef ctx, LynxFunctionObject* object) {
        JSCContext* context = static_cast<JSCContext*>(JSObjectGetPrivate(JSContextGetGlobalObject(ctx)));
        ClassWrap* class_wrap = const_cast<ClassWrap*>(context->context_storage()->GetEmptyClassWrap());
        JSObjectRef js_object = FunctionObject::Create(context, class_wrap, object);
        return js_object;
    }

    JSObjectRef JSCHelper::ConvertToObjectWrap(JSContextRef ctx, ObjectWrap* object) {
        return object->js_ref();
    }
}
