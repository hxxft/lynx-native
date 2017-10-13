// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_RUNTIME_JSC_JSC_HELPER_H_
#define LYNX_RUNTIME_JSC_JSC_HELPER_H_

#include <JavaScriptCore/JavaScript.h>
#include <string>
#include "runtime/base/lynx_array.h"
#include "runtime/base/lynx_value.h"
#include "runtime/base/lynx_object.h"
#include "runtime/base/lynx_object_template.h"
#include "config/global_config_data.h"

#define PixelToSP(value) (value / \
config::GlobalConfigData::GetInstance()->screen_density())

#define SPToPixel(value) (value * \
config::GlobalConfigData::GetInstance()->screen_density())

namespace jscore {
    class JSCHelper {
        public:
        static bool ConvertToBool(JSContextRef ctx, JSValueRef value) {
            return JSValueToBoolean(ctx, value);
        }
        static std::string ConvertToString(JSContextRef ctx, JSValueRef value);
        static std::string ConvertToString(JSContextRef ctx, JSStringRef value);
        static std::string ConvertToString(LynxValue* value);
        static base::ScopedPtr<LynxValue> ConvertToLynxValue(JSContextRef ctx, JSValueRef value);
        static base::ScopedPtr<LynxArray> ConvertToLynxArray(JSContextRef ctx, JSObjectRef value);
        static base::ScopedPtr<LynxArray> ConvertToLynxArray(JSContextRef ctx, JSValueRef *value, int length);
        static base::ScopedPtr<LynxObject> ConvertToLynxObject(JSContextRef ctx, JSObjectRef value);
        static LynxObjectTemplate* ConvertToLynxObjectTemplate(JSContextRef ctx, JSObjectRef value);
        static base::ScopedPtr<LynxValue> ConvertToLynxFunction(JSContextRef ctx, JSObjectRef value);

        static JSValueRef ConvertToJSString(JSContextRef ctx, const std::string &s);
        static JSValueRef ConvertToJSString(JSContextRef ctx, const char* s);

        static JSValueRef* ConvertToJSValueRefArray(JSContextRef ctx, jscore::LynxArray* args);
        static JSObjectRef ConvertToJSArray(JSContextRef ctx, jscore::LynxArray* args);
        static JSValueRef ConvertToJSValue(JSContextRef ctx, jscore::LynxValue* value);
        static JSObjectRef ConvertToJSObject(JSContextRef ctx, jscore::LynxObject* object);
        static JSObjectRef ConvertToJSObject(JSContextRef ctx, jscore::LynxObjectTemplate *object);
        static JSObjectRef ConvertToJSFunctionObject(JSContextRef ctx, jscore::LynxFunctionObject* object);
        static JSObjectRef ConvertToObjectWrap(JSContextRef ctx, ObjectWrap* object);

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
