// Copyright 2017 The Lynx Authors. All rights reserved.

#include "location_object.h"
#include "runtime/jsc/jsc_helper.h"
#include "runtime/jsc/jsc_context.h"
#include "runtime/jsc/class_wrap.h"
#include "base/scoped_ptr.h"
#include "base/debug/memory_debug.h"

namespace jscore {
    
    LocationObject::LocationObject(JSCContext* context, Location* location)
            : ObjectWrap(context), target_(location) {
        
    }
    
    LocationObject::~LocationObject() {
        
    }
    
    static JSValueRef GetHashCallback(JSContextRef ctx, JSObjectRef object, JSStringRef propertyName, JSValueRef* exception) {
        
        LocationObject* location = ObjectWrap::Unwrap<LocationObject>(object);
        JSStringRef str = JSStringCreateWithUTF8CString(location->target()->hash().c_str());
        JSValueRef result = JSValueMakeString(ctx, str);
        JSStringRelease(str);
        return result;
    }
    
    static bool SetHashCallback(JSContextRef ctx, JSObjectRef object, JSStringRef propertyName, JSValueRef value, JSValueRef* exception) {
        
        LocationObject* location = ObjectWrap::Unwrap<LocationObject>(object);
        location->target()->set_hash(JSCHelper::ConvertToString(ctx, value));
        return true;
    }
    
    static JSValueRef GetHostCallback(JSContextRef ctx, JSObjectRef object, JSStringRef propertyName, JSValueRef* exception) {
        
        LocationObject* location = ObjectWrap::Unwrap<LocationObject>(object);
        JSStringRef str = JSStringCreateWithUTF8CString(location->target()->host().c_str());
        JSValueRef result = JSValueMakeString(ctx, str);
        JSStringRelease(str);
        return result;
    }
    
    static bool SetHostCallback(JSContextRef ctx, JSObjectRef object, JSStringRef propertyName, JSValueRef value, JSValueRef* exception) {
        
        LocationObject* location = ObjectWrap::Unwrap<LocationObject>(object);
        location->target()->set_host(JSCHelper::ConvertToString(ctx, value));
        return true;
    }
    
    static JSValueRef GetHostNameCallback(JSContextRef ctx, JSObjectRef object, JSStringRef propertyName, JSValueRef* exception) {
        
        LocationObject* location = ObjectWrap::Unwrap<LocationObject>(object);
        JSStringRef str = JSStringCreateWithUTF8CString(location->target()->hostname().c_str());
        JSValueRef result = JSValueMakeString(ctx, str);
        JSStringRelease(str);
        return result;
    }
    
    static bool SetHostNameCallback(JSContextRef ctx, JSObjectRef object, JSStringRef propertyName, JSValueRef value, JSValueRef* exception) {
        
        LocationObject* location = ObjectWrap::Unwrap<LocationObject>(object);
        location->target()->set_hostname(JSCHelper::ConvertToString(ctx, value));
        return true;
    }
    
    static JSValueRef GetHrefCallback(JSContextRef ctx, JSObjectRef object, JSStringRef propertyName, JSValueRef* exception) {
        LocationObject* location = ObjectWrap::Unwrap<LocationObject>(object);
        JSStringRef str = JSStringCreateWithUTF8CString(location->target()->href().c_str());
        JSValueRef result = JSValueMakeString(ctx, str);
        JSStringRelease(str);
        return result;
    }
    
    static bool SetHrefCallback(JSContextRef ctx, JSObjectRef object, JSStringRef propertyName, JSValueRef value, JSValueRef* exception) {
        
        LocationObject* location = ObjectWrap::Unwrap<LocationObject>(object);
        location->target()->set_href(JSCHelper::ConvertToString(ctx, value));
        return true;
    }
    
    static JSValueRef GetPathNameCallback(JSContextRef ctx, JSObjectRef object, JSStringRef propertyName, JSValueRef* exception) {
        LocationObject* location = ObjectWrap::Unwrap<LocationObject>(object);
        JSStringRef str = JSStringCreateWithUTF8CString(location->target()->pathname().c_str());
        JSValueRef result = JSValueMakeString(ctx, str);
        JSStringRelease(str);
        return result;
    }
    
    static bool SetPathNameCallback(JSContextRef ctx, JSObjectRef object, JSStringRef propertyName, JSValueRef value, JSValueRef* exception) {
        
        LocationObject* location = ObjectWrap::Unwrap<LocationObject>(object);
        location->target()->set_pathname(JSCHelper::ConvertToString(ctx, value));
        return true;
    }
    
    static JSValueRef GetPortCallback(JSContextRef ctx, JSObjectRef object, JSStringRef propertyName, JSValueRef* exception) {
        LocationObject* location = ObjectWrap::Unwrap<LocationObject>(object);
        JSStringRef str = JSStringCreateWithUTF8CString(location->target()->port().c_str());
        JSValueRef result = JSValueMakeString(ctx, str);
        JSStringRelease(str);
        return result;
    }
    
    static bool SetPortCallback(JSContextRef ctx, JSObjectRef object, JSStringRef propertyName, JSValueRef value, JSValueRef* exception) {
        
        LocationObject* location = ObjectWrap::Unwrap<LocationObject>(object);
        location->target()->set_port(JSCHelper::ConvertToString(ctx, value));
        return true;
    }
    
    static JSValueRef GetProtocolCallback(JSContextRef ctx, JSObjectRef object, JSStringRef propertyName, JSValueRef* exception) {
        LocationObject* location = ObjectWrap::Unwrap<LocationObject>(object);
        JSStringRef str = JSStringCreateWithUTF8CString(location->target()->protocol().c_str());
        JSValueRef result = JSValueMakeString(ctx, str);
        JSStringRelease(str);
        return result;
    }
    
    static bool SetProtocolCallback(JSContextRef ctx, JSObjectRef object, JSStringRef propertyName, JSValueRef value, JSValueRef* exception) {
        
        LocationObject* location = ObjectWrap::Unwrap<LocationObject>(object);
        location->target()->set_protocol(JSCHelper::ConvertToString(ctx, value));
        return true;
    }
    
    static JSValueRef GetSearchCallback(JSContextRef ctx, JSObjectRef object, JSStringRef propertyName, JSValueRef* exception) {
        LocationObject* location = ObjectWrap::Unwrap<LocationObject>(object);
        JSStringRef str = JSStringCreateWithUTF8CString(location->target()->search().c_str());
        JSValueRef result = JSValueMakeString(ctx, str);
        JSStringRelease(str);
        return result;
    }
    
    static bool SetSearchCallback(JSContextRef ctx, JSObjectRef object, JSStringRef propertyName, JSValueRef value, JSValueRef* exception) {
        
        LocationObject* location = ObjectWrap::Unwrap<LocationObject>(object);
        location->target()->set_search(JSCHelper::ConvertToString(ctx, value));
        return true;
    }
    
    static JSValueRef GetOriginCallback(JSContextRef ctx, JSObjectRef object, JSStringRef propertyName, JSValueRef* exception) {
        LocationObject* location = ObjectWrap::Unwrap<LocationObject>(object);
        JSStringRef str = JSStringCreateWithUTF8CString(location->target()->origin().c_str());
        JSValueRef result = JSValueMakeString(ctx, str);
        JSStringRelease(str);
        return result;
    }
    
    static bool SetOriginCallback(JSContextRef ctx, JSObjectRef object, JSStringRef propertyName, JSValueRef value, JSValueRef* exception) {
        
        LocationObject* location = ObjectWrap::Unwrap<LocationObject>(object);
        location->target()->set_origin(JSCHelper::ConvertToString(ctx, value));
        return true;
    }
    
    static JSValueRef ReloadCallback(JSContextRef ctx, JSObjectRef function, JSObjectRef thiz, size_t argc, const JSValueRef argv[], JSValueRef *exception) {
        
        if (argc > 0 && JSValueIsBoolean(ctx, argv[0])) {
            LocationObject* location = ObjectWrap::Unwrap<LocationObject>(thiz);
            location->target()->Reload(JSValueToBoolean(ctx, argv[0]));
        }
        return JSValueMakeNull(ctx);
    }
    
    static JSValueRef ReplaceCallback(JSContextRef ctx, JSObjectRef function, JSObjectRef thiz, size_t argc, const JSValueRef argv[], JSValueRef *exception) {
        if (argc > 0) {
            std::string url = JSCHelper::ConvertToString(ctx, argv[0]);
            LocationObject* location = ObjectWrap::Unwrap<LocationObject>(thiz);
            location->target()->Replace(url);
        }
        return JSValueMakeNull(ctx);
    }
    
    static JSValueRef AssignCallback(JSContextRef ctx, JSObjectRef function, JSObjectRef thiz, size_t argc, const JSValueRef argv[], JSValueRef *exception) {
        if (argc > 0) {
            std::string url = JSCHelper::ConvertToString(ctx, argv[0]);
            LocationObject* location = ObjectWrap::Unwrap<LocationObject>(thiz);
            location->target()->Assign(url);
        }
        return JSValueMakeNull(ctx);
    }
    
    static JSStaticValue s_location_values_[] = {
        {"hash", GetHashCallback, SetHashCallback, kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
        {"host", GetHostCallback, SetHostCallback, kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
        {"hostname", GetHostNameCallback, SetHostNameCallback, kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
        {"href", GetHrefCallback, SetHrefCallback, kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
        {"pathname", GetPathNameCallback, SetPathNameCallback, kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
        {"port", GetPortCallback, SetPortCallback, kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
        {"protocol", GetProtocolCallback, SetProtocolCallback, kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
        {"search", GetSearchCallback, SetSearchCallback, kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
        {"origin", GetOriginCallback, SetOriginCallback, kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
        { 0, 0, 0, 0 }
    };
    
    static JSStaticFunction s_location_function_[] = {
        {"replace", ReplaceCallback, kJSClassAttributeNone},
        {"reload", ReloadCallback, kJSClassAttributeNone},
        {"assign", AssignCallback, kJSClassAttributeNone},
        { 0, 0, 0 }
    };
    
    void LocationObject::BindingClass(ClassWrap* class_wrap) {
        
        class_wrap->SetJSClassAttributes(kJSClassAttributeNone);
        class_wrap->SetJSStaticValues(s_location_values_);
        class_wrap->SetJSStaticFunctions(s_location_function_);
    }
    
    JSObjectRef LocationObject::Create(JSCContext* context, ClassWrap* class_wrap) {
        LocationObject* location = lynx_new LocationObject(context, lynx_new Location(context));
        JSObjectRef object = class_wrap->MakeObject(context->GetContext());
        ObjectWrap::Wrap(location, object);
        return object;
    }
}
