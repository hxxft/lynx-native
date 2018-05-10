// Copyright 2017 The Lynx Authors. All rights reserved.
#include "runtime/jsc/object_wrap.h"

#include "runtime/jsc/jsc_context.h"
#include "runtime/jsc/jsc_helper.h"
#include "runtime/base/lynx_object.h"

namespace jscore {

    JSCObjectWrap::JSCObjectWrap(JSCContext *context, LynxObject *obj, JSObjectRef js_obj)
            : ObjectWrap(context, obj), js_object_(js_obj) {
        assert(JSObjectSetPrivate(js_obj, this));
        OnJSObjectInitialize();
    }

    JSCObjectWrap::~JSCObjectWrap() {
    }

    void JSCObjectWrap::FinalizeCallback(JSObjectRef object) {
        void* data = JSObjectGetPrivate(object);
        JSCObjectWrap* object_wrap = static_cast<JSCObjectWrap*>(data);
        if(object_wrap != NULL) {
            object_wrap->OnJSObjectFinalize();
        }
    }

    void JSCObjectWrap::Update() {
        if(js_object_) {
            assert(JSObjectSetPrivate(js_object_, NULL));
        }
        lynx_delete(this);
    }

    void JSCObjectWrap::OnJSObjectInitialize() {
        lynx_object_->SetObjectWrap(this);
    }

    void JSCObjectWrap::OnJSObjectFinalize() {
        assert(JSObjectSetPrivate(js_object_, NULL));
        lynx_delete(this);
    }

    void JSCObjectWrap::RegisterMethodCallback(const std::string& method_name,
                                               JSMethodCallback callback) {
        auto jsc_context = static_cast<JSCContext*>(context_);
        JSCHelper::SetFunctionProperty(jsc_context->GetContext(),
                                       js_ref(),
                                       method_name,
                                       callback,
                                       kJSPropertyAttributeNone,
                                       NULL);
    }

    void JSCObjectWrap::Protect() {
        if (js_object_ != NULL) {
            auto jsc_context = static_cast<JSCContext*>(context_);
            JSContextRef context = jsc_context->GetContext();
            JSValueProtect(context, js_object_);
        }
    }

    void JSCObjectWrap::Unprotect() {
        if (js_object_ != NULL) {
            auto jsc_context = static_cast<JSCContext*>(context_);
            JSContextRef context = jsc_context->GetContext();
            if (context != NULL)
                JSValueUnprotect(context, js_object_);
        }
    }

}
