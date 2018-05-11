// Copyright 2017 The Lynx Authors. All rights reserved.

#include "runtime/jsc/jsc_context.h"

#include "runtime/runtime.h"
#include "runtime/global.h"
#include "runtime/base/lynx_value.h"
#include "runtime/base/lynx_map.h"
#include "runtime/base/lynx_object_platform.h"
#include "runtime/js/class_template.h"
#include "runtime/jsc/jsc_helper.h"
#include "runtime/jsc/prototype_builder.h"
#include "runtime/jsc/object_wrap.h"
#include "base/log/logging.h"

namespace jscore {

    JSCContext::JSCContext() : JSContext() {

    }

    JSCContext::~JSCContext() {
        // Global should be release before context release
        global_->Release();
        global_ = NULL;
        JSGlobalContextRef temp = context_;
        context_ = NULL;
        JSGlobalContextRelease(temp);
    }
    
    void JSCContext::Initialize(JSVM* vm, Runtime* runtime) {
        DLOG(INFO) << "JSCContext Initialize";
        JSContext::Initialize(vm, runtime);
        JSContextGroupRef context_group = static_cast<JSContextGroupRef>(vm->vm());
        
        global_ = lynx_new Global(this);
        global_->AddRef();
        JSCPrototypeBuilder *global_prototype_builder = static_cast<JSCPrototypeBuilder*>(
                global_->class_template()->prototype_builder());
        global_prototype_builder->SetJSClassAttributes(kJSClassAttributeNoAutomaticPrototype);
        context_ = JSGlobalContextCreateInGroup(context_group, global_prototype_builder->class_ref());
        
        JSObjectRef global_object = JSContextGetGlobalObject(context_);
        JSCObjectWrap::Wrap(this, global(), global_object);
        
        JSCHelper::SetValueProperty(context_, global_object, "window", global_object,
                                    kJSPropertyAttributeNone, 0);
        JSCHelper::SetValueProperty(context_, global_object, "global", global_object,
                                    kJSPropertyAttributeNone, 0);
    }
    
    std::string JSCContext::RunScript(const char* source) {
        JSStringRef js_source = JSStringCreateWithUTF8CString(source);
        JSValueRef exception = nullptr;

        JSValueRef result = 0;
        if (JSCheckScriptSyntax(context_, js_source, NULL, 0, &exception)) {
            result = JSEvaluateScript(context_, js_source, NULL, NULL, 0, &exception);
        }
        
        JSStringRelease(js_source);
        
        if (exception) {

            base::ScopedPtr<LynxMap> detail =
                    jscore::JSCHelper::ConvertToLynxMap(context_, (JSObjectRef) exception);
            auto line = detail->GetProperty("line");
            auto column = detail->GetProperty("column");

            std::string str = JSCHelper::ConvertToString(context_, exception);
            if (!str.empty()) {
                int line_number = line ? line->data_.i : -1;
                int column_number = column ? column->data_.i : -1;
                DLOG(ERROR) << "JS Compile ERROR: " << str << "(" << line_number << ":"<<column_number << ")";
                OnExceptionOccurred(str);
            }
        }

        return JSCHelper::ConvertToString(context_, result);
    }

    void JSCContext::LoadUrl(const std::string& url) {

    }

    void JSCContext::AddJavaScriptInterface(const std::string &name,
                                            base::ScopedPtr<LynxObjectPlatform> object) {
        object->Attach(this);
        JSObjectRef js_object = JSCHelper::ConvertToJSObject(context_, object.Release());
        JSObjectRef global = JSContextGetGlobalObject(context_);
        JSCHelper::SetValueProperty(context_,
                                    global,
                                    name,
                                    js_object,
                                    kJSPropertyAttributeReadOnly,
                                    NULL);
    }

}
