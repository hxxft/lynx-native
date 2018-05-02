// Copyright 2017 The Lynx Authors. All rights reserved.

#if USING_V8
#include "runtime/v8/objects/document_object.h"

#include <string>

#include "render/render_object.h"
#include "render/render_factory.h"
#include "runtime/v8/objects/element_object.h"
#include "runtime/v8/v8_helper.h"
#include "runtime/v8/v8_context.h"

namespace jscore {

DocumentObject::DocumentObject() {
}

DocumentObject::~DocumentObject() {
}

void DocumentObject::BindingClass(
    v8::Isolate* isolate,
    v8::Local<v8::ObjectTemplate>& globalObjectTemplate,
    V8Context* context,
    v8::Persistent<v8::FunctionTemplate>& constructor) {

    // Prepare constructor template
    auto extendData = v8:: External::New(isolate, context);
    v8::Local<v8::FunctionTemplate> tpl =
        v8::FunctionTemplate::New(isolate, NewDocumentCallback, extendData);
    tpl->SetClassName(V8Helper::ConvertToV8String(isolate, "Document"));
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    // Prototype
    tpl->PrototypeTemplate()->Set(
        V8Helper::ConvertToV8String(isolate, "createElement"),
        v8::FunctionTemplate::New(isolate, CreateElementCallback, extendData));
    tpl->PrototypeTemplate()->Set(
        V8Helper::ConvertToV8String(isolate, "createDom"),
        v8::FunctionTemplate::New(isolate, CreateDomCallback));
    tpl->PrototypeTemplate()->Set(
        V8Helper::ConvertToV8String(isolate, "addEventListener"),
        v8::FunctionTemplate::New(
            isolate,
            AddEventListenerCallback,
            extendData));
    tpl->PrototypeTemplate()->Set(
        V8Helper::ConvertToV8String(isolate, "removeEventListener"),
        v8::FunctionTemplate::New(
            isolate,
            RemoveEventListenerCallback,
            extendData));
    tpl->PrototypeTemplate()->Set(
        V8Helper::ConvertToV8String(isolate, "dispatchEvent"),
        v8::FunctionTemplate::New(isolate, DispatchEventCallback));
    tpl->PrototypeTemplate()->Set(
        V8Helper::ConvertToV8String(isolate, "createEvent"),
        v8::FunctionTemplate::New(isolate, CreateEventCallback, extendData));
    tpl->PrototypeTemplate()->SetAccessor(
        V8Helper::ConvertToV8String(isolate, "domain"),
        GetDomainCallback,
        SetDomainCallback);
    tpl->PrototypeTemplate()->SetAccessor(
        V8Helper::ConvertToV8String(isolate, "cookie"),
        GetCookieCallback,
        SetCookieCallback);

    globalObjectTemplate->Set(
        V8Helper::ConvertToV8String(isolate, "Document"),
        tpl);

    constructor.Reset(isolate, tpl);
}


v8::Local<v8::Value> DocumentObject::Create(
    v8::Isolate* isolate,
    v8::Persistent<v8::FunctionTemplate>& constructor) {
    v8::Local<v8::Context> context = isolate->GetCurrentContext();
    v8::Local<v8::FunctionTemplate> cons_tpl =
        v8::Local<v8::FunctionTemplate>::New(isolate, constructor);
    auto cons = cons_tpl->GetFunction(context).ToLocalChecked();
    // 调用v8的对象构造器,传入javaobject对象
    v8::Local<v8::Object> result =
        cons->NewInstance(context, 0, nullptr).ToLocalChecked();
    return result;
}

void DocumentObject::NewDocumentCallback(
    const v8::FunctionCallbackInfo<v8::Value>& args) {
    if (args.IsConstructCall()) {
        DocumentObject* obj = lynx_new DocumentObject();
        obj->Wrap(args.This());
        args.GetReturnValue().Set(args.This());
    } else {
    }
}

void DocumentObject::CreateElementCallback(
    const v8::FunctionCallbackInfo<v8::Value>& args) {

    v8::Isolate* isolate = args.GetIsolate();

    std::string tag = V8Helper::ConvertToString(
                          v8::Local<v8::String>::Cast(args[0]));
    V8Context* extData =
        reinterpret_cast<V8Context*>(args.Data().As<v8::External>()->Value());
    lynx::RenderObject* render_object = lynx::RenderFactory::CreateRenderObject(
                                      extData->runtime()->thread_manager(),
                                      tag,
                                      extData->runtime()->render_tree_host());
    if (render_object != NULL) {
        auto value = ElementObject::CreateElement(
                         isolate,
                         render_object,
                         extData->element_constructor());
        args.GetReturnValue().Set(value);
    } else {
        args.GetReturnValue().SetUndefined();
    }
}

void DocumentObject::CreateDomCallback(
    const v8::FunctionCallbackInfo<v8::Value>& args) {
    /*
    Isolate* isolate = args.GetIsolate();

    DocumentObject* document_obj = ObjectWrap::Unwrap<DocumentObject>(args.Holder());
    ElementObject* element_obj = ObjectWrap::Unwrap<ElementObject>(args[0]->ToObject());

    Document* document = document_obj->Document();
    Element* element = element_obj->Element();

    if(document != NULL) {
        document->CreateDom(element);
    }
    */
}

void DocumentObject::AddEventListenerCallback(
    const v8::FunctionCallbackInfo<v8::Value>& args) {
    /*
    Isolate* isolate = args.GetIsolate();

    DocumentObject* obj = ObjectWrap::Unwrap<DocumentObject>(args.Holder());
    std::string event = V8Helper::ConvertToString(args[0]);
    */
}

void DocumentObject::RemoveEventListenerCallback(
    const v8::FunctionCallbackInfo<v8::Value>& args) {
    /*
    Isolate* isolate = args.GetIsolate();

    DocumentObject* obj = ObjectWrap::Unwrap<DocumentObject>(args.Holder());
    std::string event = V8Helper::ConvertToString(args[0]);
    */
}

void DocumentObject::DispatchEventCallback(
    const v8::FunctionCallbackInfo<v8::Value>& args) {
    /*
    Isolate* isolate = args.GetIsolate();

    DocumentObject* obj = ObjectWrap::Unwrap<DocumentObject>(args.Holder());
    if (args[0]->IsString() || args[0]->IsStringObject()) {
        std::string event = V8Helper::ConvertToString(args[0]);

    }
    if (args[0]->IsObject()) {
        JSEventObject* eventObj = ObjectWrap::Unwrap<JSEventObject>(args[0]->ToObject());

    }
     */
}

void DocumentObject::CreateEventCallback(
    const v8::FunctionCallbackInfo<v8::Value>& args) {

    /*
    Isolate* isolate = args.GetIsolate();
    DocumentObject* obj = ObjectWrap::Unwrap<DocumentObject>(args.Holder());
    jstring event = V8Helper::ConvertToJString(args[0]);
    jobject result = jEnv.CallObjectMethod(obj->java_object(), create_event_method_, event);
    if(result != 0){
        JSRuntime* extData = reinterpret_cast<JSRuntime*>(args.Data().As<External>()->Value());
        auto value = JSEventObject::create(isolate,result,extData->get_jsevent_constructor());
        args.GetReturnValue().Set(value);
    }else{
        args.GetReturnValue().SetUndefined();
    }
     */
}

void DocumentObject::SetDomainCallback(
    v8::Local<v8::String> property,
    v8::Local<v8::Value> value,
    const v8::PropertyCallbackInfo<void>& info) {
    /*

    Isolate* isolate = info.GetIsolate();

    DocumentObject* obj = JavaObjectWrap::Unwrap<DocumentObject>(info.Holder());
    jstring domain = V8Helper::ConvertToJString(value);
    jEnv.CallVoidMethod(obj->java_object(), set_domain_method_, domain);
    jEnv.DeleteLocalRef(domain);
     */
}

void DocumentObject::GetDomainCallback(
    v8::Local<v8::String> property,
    const v8::PropertyCallbackInfo<v8::Value>& info) {
    /*
    Isolate* isolate = info.GetIsolate();
    DocumentObject* obj = JavaObjectWrap::Unwrap<DocumentObject>(info.This());
    jstring result = (jstring)jEnv.CallObjectMethod(obj->java_object(), get_domain_method_);
    info.GetReturnValue().Set(V8Helper::jstringToV8String(jEnv, result));
    jEnv.DeleteLocalRef(result);
     */
}

void DocumentObject::GetCookieCallback(
    v8::Local<v8::String> property,
    const v8::PropertyCallbackInfo<v8::Value>& info) {
    info.GetReturnValue().Set(
        V8Helper::ConvertToV8String(info.GetIsolate(), ""));
}

void DocumentObject::SetCookieCallback(
    v8::Local<v8::String> property,
    v8::Local<v8::Value> value,
    const v8::PropertyCallbackInfo<void>& info) {
    /*
    Isolate* isolate = info.GetIsolate();
    DocumentObject* obj = JavaObjectWrap::Unwrap<DocumentObject>(info.This());
    jstring cookie = V8Helper::ConvertToJString(value);
    jEnv.CallVoidMethod(obj->java_object(), set_cookie_method_, cookie);
    jEnv.DeleteLocalRef(cookie);
     */
}

}  // namespace jscore
#endif
