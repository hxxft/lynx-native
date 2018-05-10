// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_RUNTIME_JSC_JSC_HELPER_H_
#define LYNX_RUNTIME_JSC_JSC_HELPER_H_

#include <JavaScriptCore/JavaScript.h>
#include <string>
#include "runtime/base/lynx_value.h"

namespace jscore {
    class LynxObject;

    class JSCHelper {
        public:
        static bool ConvertToBool(JSContextRef ctx, JSValueRef value) {
            return JSValueToBoolean(ctx, value);
        }
        static std::string ConvertToString(JSContextRef ctx, JSValueRef value);
        static std::string ConvertToString(JSContextRef ctx, JSStringRef value);
        static base::ScopedPtr<LynxValue> ConvertToLynxValue(JSContextRef ctx, JSValueRef value);
        static base::ScopedPtr<LynxArray> ConvertToLynxArray(JSContextRef ctx, JSObjectRef value);
        static base::ScopedPtr<LynxArray> ConvertToLynxArray(JSContextRef ctx, JSValueRef *value, size_t length);
        static base::ScopedPtr<LynxMap> ConvertToLynxMap(JSContextRef ctx, JSObjectRef value);
        static LynxObject* ConvertToLynxObject(JSContextRef ctx, JSObjectRef value);
        static base::ScopedPtr<LynxValue> ConvertToLynxFunction(JSContextRef ctx, JSObjectRef value);

        static JSValueRef ConvertToJSString(JSContextRef ctx, const std::string &s);
        static JSValueRef ConvertToJSString(JSContextRef ctx, const char* s);

        static JSValueRef* ConvertToJSValueRefArray(JSContextRef ctx, LynxArray* args);
        static JSObjectRef ConvertToJSArray(JSContextRef ctx, LynxArray* args);
        static JSValueRef ConvertToJSValue(JSContextRef ctx, LynxValue* value);
        static JSObjectRef ConvertToJSObject(JSContextRef ctx, LynxMap* object);
        static JSObjectRef ConvertToJSObject(JSContextRef ctx, LynxObject *object);

        static void SetFunctionProperty(JSContextRef context,
                                        JSObjectRef object,
                                        const std::string& propertyName,
                                        JSObjectCallAsFunctionCallback callAsFunction,
                                        JSPropertyAttributes attributes,
                                        JSValueRef *exception) {
            
            JSStringRef name = JSStringCreateWithUTF8CString(propertyName.c_str());
            JSObjectRef function = JSObjectMakeFunctionWithCallback(context, name, callAsFunction);
            JSObjectSetProperty(context, object, name, function, attributes, exception);
            JSStringRelease(name);
        }
        
        static void SetValueProperty(JSContextRef context,
                                     JSObjectRef object,
                                     const std::string& propertyName,
                                     JSValueRef value,
                                     JSPropertyAttributes attributes,
                                     JSValueRef *exception) {
            
            JSStringRef name = JSStringCreateWithUTF8CString(propertyName.c_str());
            JSObjectSetProperty(context, object, name, value, attributes, exception);
            JSStringRelease(name);
        }

        static void DeleteProperty(JSContextRef context,
                                       JSObjectRef object,
                                       const std::string& propertyName,
                                       JSValueRef* exception) {
            JSStringRef name = JSStringCreateWithUTF8CString(propertyName.c_str());
            JSObjectDeleteProperty(context, object, name, exception);
            JSStringRelease(name);
        }
        
        static JSValueRef GetProperty(JSContextRef context,
                                JSObjectRef object,
                                const std::string& propertyName,
                                JSValueRef* exception) {
            JSStringRef name = JSStringCreateWithUTF8CString(propertyName.c_str());
            JSValueRef result = JSObjectGetProperty(context, object, name, exception);
            JSStringRelease(name);
            return result;
        }

        inline static JSValueRef ConvertToJSInt(JSContextRef ctx, LynxValue* value) {
            return JSValueMakeNumber(ctx, value->data_.i);
        }

        inline static JSValueRef ConvertToJSLong(JSContextRef ctx, LynxValue* value) {
            return JSValueMakeNumber(ctx, value->data_.l);
        }

        inline static JSValueRef ConvertToJSFloat(JSContextRef ctx, LynxValue* value) {
            return JSValueMakeNumber(ctx, value->data_.f);
        }

        inline static JSValueRef ConvertToJSDouble(JSContextRef ctx, LynxValue* value) {
            return JSValueMakeNumber(ctx, value->data_.d);
        }

        inline static JSValueRef ConvertToJSBoolean(JSContextRef ctx, LynxValue* value) {
            return JSValueMakeBoolean(ctx, value->data_.b);
        }

        inline static JSValueRef ConvertToJSString(JSContextRef ctx, LynxValue* value) {
            JSStringRef str = JSStringCreateWithUTF8CString(value->data_.str);
            JSValueRef result = JSValueMakeString(ctx, str);
            JSStringRelease(str);
            return result;
        }
    };
}

#endif // LYNX_RUNTIME_JSC_JSC_HELPER_H_
