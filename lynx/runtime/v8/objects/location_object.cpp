// Copyright 2017 The Lynx Authors. All rights reserved.

#if USING_V8

#include "runtime/v8/objects/location_object.h"

#include "net/uri.h"
#include "runtime/config.h"
#include "runtime/v8/v8_helper.h"
#include "runtime/v8/v8_context.h"
#include "runtime/v8/objects/history_object.h"

namespace jscore {

LocationObject::LocationObject(V8Context* context) : Location(context) {
}

LocationObject::~LocationObject() {
}

void LocationObject::BindingClass(
    v8::Isolate* isolate,
    v8::Local <v8::ObjectTemplate> &globalObjectTemplate,
    V8Context* context,
    v8::Persistent<v8::FunctionTemplate>& constructor) {
    // Prepare constructor template
    auto extendData = v8::External::New(isolate, context);
    v8::Local<v8::FunctionTemplate> tpl =
        v8::FunctionTemplate::New(isolate, NewLocationCallback, extendData);
    tpl->SetClassName(V8Helper::ConvertToV8String(isolate, "Location"));
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    // Prototype
    tpl->PrototypeTemplate()->SetAccessor(
        V8Helper::ConvertToV8String(isolate, "hash"),
        GetHashCallback,
        SetHashCallback);
    tpl->PrototypeTemplate()->SetAccessor(
        V8Helper::ConvertToV8String(isolate, "host"),
        GetHostCallback,
        SetHostCallback);
    tpl->PrototypeTemplate()->SetAccessor(
        V8Helper::ConvertToV8String(isolate, "hostname"),
        GetHostNameCallback,
        SetHostNameCallback);
    tpl->PrototypeTemplate()->SetAccessor(
        V8Helper::ConvertToV8String(isolate, "href"),
        GetHrefCallback,
        SetHrefCallback);
    tpl->PrototypeTemplate()->SetAccessor(
        V8Helper::ConvertToV8String(isolate, "pathname"),
        GetPathNameCallback,
        SetPathNameCallback);
    tpl->PrototypeTemplate()->SetAccessor(
        V8Helper::ConvertToV8String(isolate, "port"),
        GetPortCallback,
        SetPortCallback);
    tpl->PrototypeTemplate()->SetAccessor(
        V8Helper::ConvertToV8String(isolate, "protocol"),
        GetProtocolCallback,
        SetProtocolCallback);
    tpl->PrototypeTemplate()->SetAccessor(
        V8Helper::ConvertToV8String(isolate, "search"),
        GetSearchCallback,
        SetSearchCallback);
    tpl->PrototypeTemplate()->SetAccessor(
        V8Helper::ConvertToV8String(isolate, "origin"),
        GetOriginCallback,
        SetOriginCallback);

    tpl->PrototypeTemplate()->Set(
            V8Helper::ConvertToV8String(isolate, "replace"),
            v8::FunctionTemplate::New(isolate, ReplaceCallback));
    tpl->PrototypeTemplate()->Set(
            V8Helper::ConvertToV8String(isolate, "reload"),
            v8::FunctionTemplate::New(isolate, ReloadCallback));
    tpl->PrototypeTemplate()->Set(
            V8Helper::ConvertToV8String(isolate, "assign"),
            v8::FunctionTemplate::New(isolate, AssignCallback));

    constructor.Reset(isolate, tpl);
    globalObjectTemplate->Set(
        V8Helper::ConvertToV8String(isolate, "Location"),
        tpl);
}

void LocationObject::NewLocationCallback(
    const v8::FunctionCallbackInfo<v8::Value>& args) {
    if (args.IsConstructCall()) {
        V8Context* context = reinterpret_cast<V8Context*>(
                                 args.Data().As<v8::External>()->Value());
        LocationObject* obj = lynx_new LocationObject(context);
        obj->Wrap(args.This());
        args.GetReturnValue().Set(args.This());
    }
}

v8::Local<v8::Value> LocationObject::Create(
    v8::Isolate* isolate,
    v8::Persistent<v8::FunctionTemplate>& constructor) {
    v8::Local<v8::Context> context = isolate->GetCurrentContext();
    v8::Local<v8::FunctionTemplate> cons_tpl =
        v8::Local<v8::FunctionTemplate>::New(isolate, constructor);
    auto cons = cons_tpl->GetFunction(context).ToLocalChecked();
    v8::Local<v8::Object> result =
        cons->NewInstance(context, 0, nullptr).ToLocalChecked();
    return result;
}

void LocationObject::ReplaceCallback(
    const v8::FunctionCallbackInfo<v8::Value>& args) {
    if (args.Length() > 0 && args[0]->IsString()) {
        v8::Isolate *isolate = args.GetIsolate();
        v8::Local<v8::Context> context = isolate->GetCurrentContext();
        LocationObject* obj = ObjectWrap::Unwrap<LocationObject>(args.Holder());
        std::string url = V8Helper::ConvertToString(
                              args[0]->ToString(context).ToLocalChecked());
        obj->Replace(url);
    }
}

void LocationObject::ReloadCallback(
    const v8::FunctionCallbackInfo<v8::Value>& args) {
    if (args.Length() > 0 && args[0]->IsBoolean()) {
        v8::Isolate *isolate = args.GetIsolate();
        v8::Local<v8::Context> context = isolate->GetCurrentContext();
        LocationObject* obj =
            ObjectWrap::Unwrap<LocationObject>(args.Holder());
        bool force = V8Helper::ConvertToBoolean(
                         args[0]->ToBoolean(context).ToLocalChecked());
        obj->Reload(force);
    }
}

void LocationObject::AssignCallback(
    const v8::FunctionCallbackInfo<v8::Value>& args) {
    if (args.Length() > 0 && args[0]->IsString()) {
        v8::Isolate *isolate = args.GetIsolate();
        v8::Local<v8::Context> context = isolate->GetCurrentContext();
        LocationObject* obj =
            ObjectWrap::Unwrap<LocationObject>(args.Holder());
        std::string url = V8Helper::ConvertToString(
                              args[0]->ToString(context).ToLocalChecked());
        obj->Assign(url);
    }
}

//void LocationObject::SetUrl(const std::string& url) {
//    ParseHref(url);
//}
//
//void LocationObject::ParseHref(const std::string& href) {
//    uri_ = net::Uri::Parse(href);
//    href_ = uri_.href_;
//    host_ = uri_.host_;
//    protocol_ = uri_.protocol_;
//    port_ = uri_.port_;
//    pathname_ = uri_.path_;
//    search_ = uri_.query_;
//    origin_ = uri_.origin_;
//    hostname_ = uri_.hostname_;
//}

void LocationObject::GetHostNameCallback(
    v8::Local<v8::String> property,
    const v8::PropertyCallbackInfo<v8::Value>& info) {
    LocationObject* obj =
        ObjectWrap::Unwrap<LocationObject>(info.This());
    info.GetReturnValue().Set(V8Helper::ConvertToV8String(
                                  info.GetIsolate(),
                                  obj->hostname()));
}

void LocationObject::SetHostNameCallback(
    v8::Local<v8::String> property,
    v8::Local<v8::Value> value,
    const v8::PropertyCallbackInfo<void>& info) {
    if (value->IsString()) {
        v8:: Isolate* isolate = info.GetIsolate();
        v8::Local<v8::Context> context = isolate->GetCurrentContext();
        LocationObject* obj = ObjectWrap::Unwrap<LocationObject>(info.This());
        obj->set_hostname(V8Helper::ConvertToString(
                             value->ToString(context).ToLocalChecked()));
    }
}

void LocationObject::GetHashCallback(
    v8::Local<v8::String> property,
    const v8::PropertyCallbackInfo<v8::Value>& info) {
    LocationObject* obj = ObjectWrap::Unwrap<LocationObject>(info.This());
    info.GetReturnValue().Set(
        V8Helper::ConvertToV8String(info.GetIsolate(), obj->hash()));
}

void LocationObject::SetHashCallback(
    v8::Local<v8::String> property,
    v8::Local<v8::Value> value,
    const v8::PropertyCallbackInfo<void>& info) {
    if (value->IsString()) {
        v8::Isolate* isolate = info.GetIsolate();
        v8::Local<v8::Context> context = isolate->GetCurrentContext();
        LocationObject* obj = ObjectWrap::Unwrap<LocationObject>(info.This());
        obj->set_hash(V8Helper::ConvertToString(
                         value->ToString(context).ToLocalChecked()));
    }
}

void LocationObject::GetHostCallback(
    v8::Local<v8::String> property,
    const v8::PropertyCallbackInfo<v8::Value>& info) {
    LocationObject* obj = ObjectWrap::Unwrap<LocationObject>(info.This());
    info.GetReturnValue().Set(
        V8Helper::ConvertToV8String(info.GetIsolate(), obj->host()));
}

void LocationObject::SetHostCallback(
    v8::Local<v8::String> property,
    v8::Local<v8::Value> value,
    const v8::PropertyCallbackInfo<void>& info) {
    if (value->IsString()) {
        v8::Isolate* isolate = info.GetIsolate();
        v8::Local<v8::Context> context = isolate->GetCurrentContext();
        LocationObject* obj = ObjectWrap::Unwrap<LocationObject>(info.This());
        obj->set_host(V8Helper::ConvertToString(
                         value->ToString(context).ToLocalChecked()));
    }
}

void LocationObject::GetHrefCallback(
    v8::Local<v8::String> property,
    const v8::PropertyCallbackInfo<v8::Value>& info) {
    LocationObject* obj = ObjectWrap::Unwrap<LocationObject>(info.This());
    info.GetReturnValue().Set(
        V8Helper::ConvertToV8String(info.GetIsolate(), obj->href()));
}

void LocationObject::SetHrefCallback(
    v8::Local<v8::String> property,
    v8::Local<v8::Value> value,
    const v8::PropertyCallbackInfo<void>& info) {
    if (value->IsString()) {
        v8::Isolate* isolate = info.GetIsolate();
        v8::Local<v8::Context> context = isolate->GetCurrentContext();
        LocationObject* obj = ObjectWrap::Unwrap<LocationObject>(info.This());
        std::string href = V8Helper::ConvertToString(
                          value->ToString(context).ToLocalChecked());
        obj->ParseHref(href);
    }
}

void LocationObject::GetPortCallback(
    v8::Local<v8::String> property,
    const v8::PropertyCallbackInfo<v8::Value>& info) {
    LocationObject* obj = ObjectWrap::Unwrap<LocationObject>(info.This());
    info.GetReturnValue().Set(
        V8Helper::ConvertToV8String(info.GetIsolate(), obj->port()));
}

void LocationObject::SetPortCallback(
    v8::Local<v8::String> property,
    v8::Local<v8::Value> value,
    const v8::PropertyCallbackInfo<void>& info) {
    if (value->IsString()) {
        v8::Isolate* isolate = info.GetIsolate();
        v8::Local<v8::Context> context = isolate->GetCurrentContext();
        LocationObject* obj = ObjectWrap::Unwrap<LocationObject>(info.This());
        obj->set_port(V8Helper::ConvertToString(
                         value->ToString(context).ToLocalChecked()));
    }
}

void LocationObject::GetPathNameCallback(
    v8::Local<v8::String> property,
    const v8::PropertyCallbackInfo<v8::Value>& info) {
    LocationObject* obj = ObjectWrap::Unwrap<LocationObject>(info.This());
    info.GetReturnValue().Set(
        V8Helper::ConvertToV8String(info.GetIsolate(), obj->pathname()));
}

void LocationObject::SetPathNameCallback(
    v8::Local<v8::String> property,
    v8::Local<v8::Value> value,
    const v8::PropertyCallbackInfo<void>& info) {
    if (value->IsString()) {
        v8::Isolate* isolate = info.GetIsolate();
        v8::Local<v8::Context> context = isolate->GetCurrentContext();
        LocationObject* obj = ObjectWrap::Unwrap<LocationObject>(info.This());
        obj->set_pathname(V8Helper::ConvertToString(
                             value->ToString(context).ToLocalChecked()));
    }
}

void LocationObject::GetSearchCallback(
    v8::Local<v8::String> property,
    const v8::PropertyCallbackInfo<v8::Value>& info) {
    LocationObject* obj = ObjectWrap::Unwrap<LocationObject>(info.This());
    info.GetReturnValue().Set(
        V8Helper::ConvertToV8String(info.GetIsolate(), obj->search()));
}

void LocationObject::SetSearchCallback(
    v8::Local<v8::String> property,
    v8::Local<v8::Value> value,
    const v8::PropertyCallbackInfo<void>& info) {
    if (value->IsString()) {
        v8::Isolate* isolate = info.GetIsolate();
        v8::Local<v8::Context> context = isolate->GetCurrentContext();
        LocationObject* obj = ObjectWrap::Unwrap<LocationObject>(info.Holder());
        obj->set_search(V8Helper::ConvertToString(
                           value->ToString(context).ToLocalChecked()));
    }
}

void LocationObject::GetProtocolCallback(
    v8::Local<v8::String> property,
    const v8::PropertyCallbackInfo<v8::Value>& info) {
    LocationObject* obj = ObjectWrap::Unwrap<LocationObject>(info.This());
    info.GetReturnValue().Set(
        V8Helper::ConvertToV8String(info.GetIsolate(), obj->protocol()));
}

void LocationObject::SetProtocolCallback(
    v8::Local<v8::String> property,
    v8::Local<v8::Value> value,
    const v8::PropertyCallbackInfo<void>& info) {
    if (value->IsString()) {
        v8::Isolate* isolate = info.GetIsolate();
        v8::Local<v8::Context> context = isolate->GetCurrentContext();
        LocationObject* obj = ObjectWrap::Unwrap<LocationObject>(info.This());
        obj->set_protocol(V8Helper::ConvertToString(
                             value->ToString(context).ToLocalChecked()));
    }
}

void LocationObject::GetOriginCallback(
    v8::Local<v8::String> property,
    const v8::PropertyCallbackInfo<v8::Value>& info) {
    LocationObject* obj = ObjectWrap::Unwrap<LocationObject>(info.This());
    info.GetReturnValue().Set(
        V8Helper::ConvertToV8String(info.GetIsolate(), obj->origin()));
}

void LocationObject::SetOriginCallback(
    v8::Local<v8::String> property,
    v8::Local<v8::Value> value,
    const v8::PropertyCallbackInfo<void>& info) {
    if (value->IsString()) {
        v8::Isolate* isolate = info.GetIsolate();
        v8::Local<v8::Context>
        context = isolate->GetCurrentContext();
        LocationObject* obj = ObjectWrap::Unwrap<LocationObject>(info.This());
        obj->set_origin(V8Helper::ConvertToString(
                           value->ToString(context).ToLocalChecked()));
    }
}
}  // namespace jscore
#endif
