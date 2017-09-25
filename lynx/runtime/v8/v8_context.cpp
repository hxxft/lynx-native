// Copyright 2017 The Lynx Authors. All rights reserved.

#include "runtime/v8/v8_context.h"

#include <sstream>

#include "base/print.h"
#include "runtime/v8/objects/body_object.h"
#include "runtime/v8/objects/console_object.h"
#include "runtime/v8/objects/document_object.h"
#include "runtime/v8/objects/navigator_object.h"
#include "runtime/v8/objects/screen_object.h"
#include "runtime/v8/objects/window_object.h"
#include "runtime/v8/objects/location_object.h"
#include "runtime/v8/objects/history_object.h"
#include "runtime/v8/objects/loader_object.h"
#include "runtime/v8/simple_allocator.h"
#include "runtime/v8/timeout_callback.h"
#include "runtime/v8/v8_helper.h"

namespace jscore {

V8Context::~V8Context() {
    Clear();
    context_.Reset();
}

void V8Context::Initialize(JSVM* vm, Runtime* runtime) {
    LOGD("lynx-debug", "V8Context::Initialize");
    JSContext::Initialize(vm, runtime);
    v8::Isolate* isolate = static_cast<v8::Isolate*>(GetVM());
    v8::Isolate::Scope isolate_scope(isolate);
    v8::HandleScope handleScope(isolate);

    // v8::V8::SetFlagsFromString(Constants::V8_STARTUP_FLAGS.c_str(),
    //                       Constants::V8_STARTUP_FLAGS.size());
    //const char* flag = "--expose_gc";
    //v8::V8::SetFlagsFromString(flag, static_cast<int>(strlen(flag)));

    v8::V8::SetCaptureStackTraceForUncaughtExceptions(
        true, 100, v8::StackTrace::kOverview);
    v8::V8::AddMessageListener(V8Context::OnUncaughtError);

    auto globalTemplate = v8::ObjectTemplate::New(isolate);

    ConsoleObject::BindingClass(isolate, globalTemplate, console_constructor_);
    NavigatorObject::BindingClass(isolate, globalTemplate, navigator_constructor_);
    ScreenObject::BindingClass(isolate, globalTemplate, screen_constructor_);
    HistoryObject::BindingClass(isolate, globalTemplate, this, history_constructor_);
    WindowObject::BindingClass(isolate, globalTemplate, this);
    LocationObject::BindingClass(isolate, globalTemplate, this, location_constructor_);
    DocumentObject::BindingClass(isolate, globalTemplate, this, document_constructor_);
    BodyObject::BindingClass(isolate, globalTemplate, this, body_constructor_);
    ElementObject::BindingClass(isolate, globalTemplate, this, element_constructor_);
    TimeoutCallback::BindingCallback(isolate, globalTemplate, this);
    LoaderObject::BindingClass(isolate, globalTemplate, this, loader_constructor_);

    v8::Local <v8::Context> context = v8::Context::New(isolate, nullptr, globalTemplate);

    context_.Reset(isolate, context);

    v8::Context::Scope context_scope(context);

    auto global = context->Global();

    auto document = DocumentObject::Create(isolate, document_constructor_);
    auto navigator = NavigatorObject::Create(isolate, navigator_constructor_);
    auto screen = ScreenObject::Create(isolate, screen_constructor_);
    auto location = LocationObject::Create(isolate, location_constructor_);
    auto console = ConsoleObject::Create(isolate, console_constructor_);
    auto history = HistoryObject::Create(isolate, history_constructor_);
    auto loader = LoaderObject::Create(isolate, loader_constructor_);

    document->ToObject(context).ToLocalChecked()->Set(context,
            V8Helper::ConvertToV8String(isolate, "body"),
            BodyObject::Create(isolate, body_constructor_)).FromJust();

    global->Set(context, V8Helper::ConvertToV8String(isolate, "console"), console).FromJust();
    global->Set(context, V8Helper::ConvertToV8String(isolate, "document"), document).FromJust();
    global->Set(context, V8Helper::ConvertToV8String(isolate, "global"), global).FromJust();
    global->Set(context, V8Helper::ConvertToV8String(isolate, "window"), global).FromJust();
    global->Set(context, V8Helper::ConvertToV8String(isolate, "navigator"), navigator).FromJust();
    global->Set(context, V8Helper::ConvertToV8String(isolate, "screen"), screen).FromJust();
    global->Set(context, V8Helper::ConvertToV8String(isolate, "location"), location).FromJust();
    global->Set(context, V8Helper::ConvertToV8String(isolate, "history"), history).FromJust();
    global->Set(context, V8Helper::ConvertToV8String(isolate, "loader"), loader).FromJust();

    document_object_ = ObjectWrap::Unwrap<DocumentObject>(v8::Local<v8::Object>::Cast(document));
    console_object_ = ObjectWrap::Unwrap<ConsoleObject>(v8::Local<v8::Object>::Cast(console));
    navigator_object_ = ObjectWrap::Unwrap<NavigatorObject>(v8::Local<v8::Object>::Cast(navigator));
    screen_object_ = ObjectWrap::Unwrap<ScreenObject>(v8::Local<v8::Object>::Cast(screen));
    location_object_ = ObjectWrap::Unwrap<LocationObject>(v8::Local<v8::Object>::Cast(location));
    history_object_ = ObjectWrap::Unwrap<HistoryObject>(v8::Local<v8::Object>::Cast(history));
    loader_object_ = ObjectWrap::Unwrap<LoaderObject>(v8::Local<v8::Object>::Cast(loader));
}

    void V8Context::Clear() {
        v8::Isolate* isolate = static_cast<v8::Isolate*>(GetVM());
        v8::Isolate::Scope isolate_scope(isolate);
        v8::HandleScope handleScope(isolate);
        v8::Local<v8::Context> context = v8::Local<v8::Context>::New(isolate, GetContext());
        Context::Scope context_scope(context);

        Local<v8::Object> global = context->Global();
        Local<v8::Array> keys = global->GetPropertyNames(context).ToLocalChecked();
        for(jsize i =0;i<keys->Length();i++){
            auto key = keys->Get(context, i).ToLocalChecked();
            global->Set(key, v8::Null(isolate));
            LOGD("DELETE","KEY = %s",V8Helper::ConvertToString(key->ToString()).c_str());
            //global->Delete(context, key);
        }
        document_constructor_.Reset();
        body_constructor_.Reset();
        location_constructor_.Reset();
        loader_constructor_.Reset();
        navigator_constructor_.Reset();
        screen_constructor_.Reset();
        console_constructor_.Reset();
        history_constructor_.Reset();
        element_constructor_.Reset();
        isolate->LowMemoryNotification();
    }

void V8Context::LoadUrl(const std::string &url) {
    location_object_->SetUrl(url);
    history_object_->Go(url);
    loader_->Load(url, loader::XCFile::XC_URL);
}

void V8Context::RunScript(const char* source) {

    v8::Isolate* isolate = static_cast<v8::Isolate*>(GetVM());
    v8::Isolate::Scope isolate_scope(isolate);
    v8::HandleScope handleScope(isolate);
    v8::Local<v8::Context> context = v8::Local<v8::Context>::New(isolate, GetContext());
    Context::Scope context_scope(context);

    v8::TryCatch tc(isolate);

    v8::MaybeLocal<v8::Script> maybe_local_script = v8::Script::Compile(context,
            v8::String::NewFromUtf8(isolate, source, v8::NewStringType::kNormal).ToLocalChecked());
    v8::Local <v8::Script> script;
    if (!maybe_local_script.IsEmpty()) {
        script = maybe_local_script.ToLocalChecked();
    }

    if (tc.HasCaught()) {
        std::string error = GetErrorMessage(tc.Message(), tc.Exception());
        LOGE("lynx", "JSRuntime error: %s", error.c_str());
    }

    if (!script.IsEmpty()) {

        auto result = script->Run(context);

        if (result.IsEmpty()) {
            //printf("JSRuntime empty result");
        }

        if (tc.HasCaught()) {
            std::string error = GetErrorMessage(tc.Message(), tc.Exception());
            LOGE("lynx", "JSRuntime error: %s", error.c_str());
        }

    }

}

std::string V8Context::GetErrorMessage(const v8::Local <v8::Message> &message, const v8::Local <v8::Value> &error) {
    std::stringstream ss;

    auto context = v8::Isolate::GetCurrent()->GetCurrentContext();

    auto str = error->ToDetailString(context).ToLocalChecked();
    if (str.IsEmpty()) {
        str = v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), "", v8::NewStringType::kNormal).ToLocalChecked();
    }
    v8::String::Utf8Value utfError(str);
    ss << std::endl << std::endl << *utfError << std::endl;
    auto scriptResName = message->GetScriptResourceName();
    if (!scriptResName.IsEmpty() && scriptResName->IsString()) {
        ss << "File: \"" << V8Helper::ConvertToString(scriptResName.As<v8::String>());
    }
    else {
        ss << "File: \"<unknown>";
    }
    ss << ", line: " << message->GetLineNumber(context).FromJust() << ", column: " << message->GetStartColumn(context).FromJust() <<
       std::endl << std::endl;

    std::string stackTraceMessage = GetErrorStackTrace(message->GetStackTrace());
    ss << "StackTrace: " << std::endl << stackTraceMessage << std::endl;

    return ss.str();
}

std::string V8Context::GetErrorStackTrace(const v8::Local<v8::StackTrace>& stackTrace)
{
    if (stackTrace.IsEmpty()) {
        return "";
    }
    std::stringstream ss;

    auto isolate = v8::Isolate::GetCurrent();
    v8::HandleScope handleScope(isolate);

    int frameCount = stackTrace->GetFrameCount();

    for (int i = 0; i < frameCount; i++)
    {
        auto frame = stackTrace->GetFrame(i);
        auto funcName = V8Helper::ConvertToString(frame->GetFunctionName());
        auto srcName = V8Helper::ConvertToString(frame->GetScriptName());
        auto lineNumber = frame->GetLineNumber();
        auto column = frame->GetColumn();

        ss << "\tFrame: function:'" << funcName.c_str() << "', file:'" << srcName.c_str() << "', line: " << lineNumber << ", column: " << column << std::endl;
    }

    return ss.str();
}

void V8Context::OnUncaughtError(v8::Handle<v8::Message> message, v8::Handle<v8::Value> error) {
    std::string m_message = GetErrorMessage(message, error);
    LOGE("NativeException", "error: %s", m_message.c_str());
}
}  // namespace jscore
