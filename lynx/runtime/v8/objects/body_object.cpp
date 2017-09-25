// Copyright 2017 The Lynx Authors. All rights reserved.

#if USING_V8

#include "config/global_config_data.h"
#include "runtime/v8/objects/body_object.h"

#include "render/body.h"
#include "runtime/v8/v8_helper.h"
#include "runtime/v8/v8_context.h"

namespace jscore {

BodyObject::BodyObject(lynx::RenderObject* render_object)
    : ElementObject(render_object) {
}

BodyObject::~BodyObject() {
}

void BodyObject::BindingClass(
    v8::Isolate* isolate,
    v8::Local<v8::ObjectTemplate> &globalObjectTemplate,
    V8Context* context,
    v8::Persistent<v8::FunctionTemplate>& constructor) {

    // 创建js element对象回调
    auto extendData = v8::External::New(isolate, context);
    v8::Local<v8::FunctionTemplate> tpl =
        v8::FunctionTemplate::New(isolate, NewBodyCallback, extendData);
    tpl->SetClassName(V8Helper::ConvertToV8String(isolate, "Body"));
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    // 增加元素js方法
    ElementObject::SetFuncTemplate(isolate, tpl, extendData);

    tpl->PrototypeTemplate()->SetAccessor(
        V8Helper::ConvertToV8String(isolate, "clientWidth"),
        GetClientWidthCallback, NULL);

    tpl->PrototypeTemplate()->SetAccessor(
        V8Helper::ConvertToV8String(isolate, "clientHeight"),
        GetClientHeightCallback, NULL);

    constructor.Reset(isolate, tpl);
    // 向js global中注册此元素
    globalObjectTemplate->Set(
        V8Helper::ConvertToV8String(isolate, "Body"), tpl);
}

// 初始化元素
void BodyObject::NewBodyCallback(
    const v8::FunctionCallbackInfo<v8::Value>& args) {
    V8Context* extData =
        reinterpret_cast<V8Context*>(args.Data().As<v8::External>()->Value());
    if (args.IsConstructCall()) {
        lynx::RenderObject* render_object =
            extData->runtime()->render_tree_host()->render_root();
        BodyObject* obj = lynx_new BodyObject(render_object);
        render_object->SetJSRef(reinterpret_cast<void*>(obj));
        obj->Wrap(args.This());
    }
}

// 创建js对象
v8::Local<v8::Value> BodyObject::Create(
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

void BodyObject::GetClientWidthCallback(
    v8::Local<v8::String> property,
    const v8::PropertyCallbackInfo <v8::Value> &info) {
    v8::Isolate *isolate = info.GetIsolate();
    info.GetReturnValue().Set(
            v8::Number::New(
                    isolate,
                    config::GlobalConfigData::GetInstance()->screen_width()));
}

void BodyObject::GetClientHeightCallback(
    v8::Local <v8::String> property,
    const v8::PropertyCallbackInfo <v8::Value> &info) {
    v8::Isolate *isolate = info.GetIsolate();
    info.GetReturnValue().Set(
            v8::Number::New(
                    isolate,
                    config::GlobalConfigData::GetInstance()->screen_height()));
}
}  // namespace jscore
#endif  // USING_V8
