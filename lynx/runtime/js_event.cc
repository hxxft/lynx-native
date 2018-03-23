// Copyright 2017 The Lynx Authors. All rights reserved.

#include "runtime/js_event.h"
#include "runtime/base/lynx_value.h"
#include "runtime/base/lynx_array.h"
#include "runtime/element.h"

namespace jscore {
    JSEvent::JSEvent(std::string event) : current_target_(NULL),
                                          target_(NULL),
                                          default_prevented_(false),
                                          cancel_bubble_(false),
                                          cancelable_(false),
                                          bubbles_(false),
                                          time_stamp(0),
                                          type_(event){
        set_class_name("JSEvent");
        RegisterMethodCallback("initEvent", &InitEventCallback);
        RegisterMethodCallback("preventDefault", &PreventDefaultCallback);
        RegisterMethodCallback("stopPropagation", &StopPropagationCallback);
        RegisterAccessorCallback("type", &GetTypeCallback, 0);
        RegisterAccessorCallback("bubbles", &GetBubblesCallback, 0);
        RegisterAccessorCallback("cancelable", &GetCancelableCallback, 0);
        RegisterAccessorCallback("target", &GetTargetCallback, 0);
        RegisterAccessorCallback("currentTarget", &GetCurrentTargetCallback, 0);
    }

    JSEvent::~JSEvent() {

    }

    void JSEvent::InitEvent(std::string type, bool bubbles, bool cancelable) {

    }

    void JSEvent::PreventDefault() {

    }

    void JSEvent::StopPropagation() {

    }

    base::ScopedPtr<LynxValue> JSEvent::GetTypeCallback(LynxObjectTemplate* object) {
        std::string type = static_cast<JSEvent*>(object)->type();
        return LynxValue::MakeString(type);
    }

    base::ScopedPtr<LynxValue> JSEvent::GetBubblesCallback(LynxObjectTemplate* object) {
        return LynxValue::MakeBool(static_cast<JSEvent*>(object)->bubbles());
    }

    base::ScopedPtr<LynxValue> JSEvent::GetCancelableCallback(LynxObjectTemplate* object) {
        return LynxValue::MakeBool(static_cast<JSEvent*>(object)->cancelable());
    }

    base::ScopedPtr<LynxValue> JSEvent::GetTargetCallback(LynxObjectTemplate* object) {
        JSEvent* js_event = static_cast<JSEvent*>(object);
        return LynxValue::MakeObjectTemplate(
                js_event->target()->GetJSRef());
    }

    base::ScopedPtr<LynxValue> JSEvent::GetCurrentTargetCallback(LynxObjectTemplate* object) {
        JSEvent* js_event = static_cast<JSEvent*>(object);
        return LynxValue::MakeObjectTemplate(
                js_event->current_target()->GetJSRef());
    }

    base::ScopedPtr<LynxValue>
    JSEvent::StopPropagationCallback(LynxObjectTemplate* object,
                                     base::ScopedPtr<LynxArray>& array) {
        static_cast<JSEvent*>(object)->StopPropagation();
        return base::ScopedPtr<LynxValue>();
    }

    base::ScopedPtr<LynxValue>
    JSEvent::PreventDefaultCallback(LynxObjectTemplate* object,
                                    base::ScopedPtr<LynxArray>& array) {
        static_cast<JSEvent*>(object)->PreventDefault();
        return base::ScopedPtr<LynxValue>();
    }

    base::ScopedPtr<LynxValue>
    JSEvent::InitEventCallback(LynxObjectTemplate* object,
                               base::ScopedPtr<LynxArray>& array) {
        static_cast<JSEvent*>(object)->InitEvent(array->Get(0)->data_.str,
                                                 array->Get(1)->data_.b,
                                                 array->Get(2)->data_.b);
        return base::ScopedPtr<LynxValue>();
    }

    void JSEvent::Reset() {

    }

}