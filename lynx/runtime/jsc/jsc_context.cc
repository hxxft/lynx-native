// Copyright 2017 The Lynx Authors. All rights reserved.

#include "runtime/base/lynx_value.h"
#include "runtime/jsc/jsc_context.h"
#include "runtime/jsc/objects/window_object.h"
#include "runtime/jsc/jsc_class_wrap_storage.h"

#include "runtime/runtime.h"
#include "runtime/navigator.h"
#include "runtime/loader.h"
#include "runtime/screen.h"
#include "runtime/document.h"
#include "runtime/console.h"

#include "runtime/jsc/timeout_callback.h"
#include "runtime/jsc/jsc_helper.h"
#include "runtime/jsc/class_wrap.h"
#include "base/log/logging.h"

namespace jscore {

    JSCContext::JSCContext() : JSContext(), class_wrap_storage_(lynx_new JSCClassWrapStorage()) {

    }

    JSCContext::~JSCContext() {
        JSGlobalContextRef temp = context_;
        context_ = NULL;
        JSGlobalContextRelease(temp);
    }
    
    void JSCContext::Initialize(JSVM* vm, Runtime* runtime) {
        DLOG(INFO) << "JSCContext Initialize";
        JSContext::Initialize(vm, runtime);
        JSContextGroupRef context_group = static_cast<JSContextGroupRef>(vm->vm());
        
        ClassWrap* global_class_wrap = class_wrap_storage_->CreateClassWrap("Global", NULL);
        TimeoutCallback::BindingClass(global_class_wrap);
        WindowObject::BindingClass(global_class_wrap);

        global_class_wrap->SetJSClassAttributes(kJSClassAttributeNoAutomaticPrototype);
        JSClassRef global_class = global_class_wrap->MakeClass();
        context_ = JSGlobalContextCreateInGroup(context_group, global_class);
        JSClassRelease(global_class);
        
        JSObjectRef global_object = JSContextGetGlobalObject(context_);
        JSObjectSetPrivate(global_object, this);

        JSObjectRef loader_object = JSCHelper::ConvertToJSObject(context_, lynx_new Loader(this));
        JSObjectRef console_object = JSCHelper::ConvertToJSObject(context_, lynx_new Console());
        JSObjectRef screen_object = JSCHelper::ConvertToJSObject(context_, lynx_new Screen());
        JSObjectRef navigator_object = JSCHelper::ConvertToJSObject(context_, lynx_new Navigator(this));
        JSObjectRef document_object = JSCHelper::ConvertToJSObject(context_, lynx_new Document(this));
        Element* body_element = lynx_new Element(this, runtime_->render_tree_host()->render_root());
        JSObjectRef body_object = JSCHelper::ConvertToJSObject(context_, body_element);

        JSCHelper::SetValueProperty(context_, global_object, "console", console_object,
                                    kJSPropertyAttributeNone, 0);
        JSCHelper::SetValueProperty(context_, global_object, "navigator", navigator_object,
                                    kJSPropertyAttributeNone, 0);
        JSCHelper::SetValueProperty(context_, global_object, "screen", screen_object,
                                    kJSPropertyAttributeNone, 0);
        JSCHelper::SetValueProperty(context_, global_object, "window", global_object,
                                    kJSPropertyAttributeNone, 0);
        JSCHelper::SetValueProperty(context_, global_object, "loader", loader_object,
                                    kJSPropertyAttributeNone, 0);
        JSCHelper::SetValueProperty(context_, global_object, "document", document_object,
                                    kJSPropertyAttributeNone, 0);
        JSCHelper::SetValueProperty(context_, document_object, "body", body_object,
                                    kJSPropertyAttributeNone, 0);
        JSCHelper::SetValueProperty(context_, global_object, "global", global_object,
                                    kJSPropertyAttributeNone, 0);
        JSCHelper::SetValueProperty(context_, global_object, "__global", global_object,
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

            base::ScopedPtr<LynxObject> detail =
                    jscore::JSCHelper::ConvertToLynxObject(context_, (JSObjectRef) exception);
            auto line = detail->GetProperty("line");
            auto column = detail->GetProperty("column");

            std::string str = JSCHelper::ConvertToString(context_, exception);
            if (!str.empty()) {
                int line_number = line ? line->data_.i : -1;
                int column_number = column ? column->data_.i : -1;
                DLOG(ERROR) << "JS Compile ERROR: " << str << "(" << line_number << ":"<<column_number << ")";
                OnExceptionOccured(str);
            }
        }

        return JSCHelper::ConvertToString(context_, result);
    }

    void JSCContext::LoadUrl(const std::string& url) {

    }

    void JSCContext::AddJavaScriptInterface(const std::string &name,
                                            LynxFunctionObject *object) {
        JSObjectRef js_object = JSCHelper::ConvertToJSFunctionObject(context_, object);
        JSObjectRef global = JSContextGetGlobalObject(context_);
        JSCHelper::SetValueProperty(context_,
                                    global,
                                    name,
                                    js_object,
                                    kJSPropertyAttributeReadOnly,
                                    NULL);
    }
}
