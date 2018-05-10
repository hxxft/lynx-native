// Copyright 2017 The Lynx Authors. All rights reserved.

#include "runtime/v8/v8_context.h"

#include <sstream>

#include "base/print-bak.h"
#include "base/log/logging.h"
#include "runtime/runtime.h"
#include "runtime/global.h"
#include "runtime/base/lynx_object_platform.h"
#include "runtime/v8/simple_allocator.h"
#include "runtime/v8/v8_helper.h"
#include "runtime/v8/prototype_builder.h"

namespace jscore {

V8Context::~V8Context() {
    Clear();
    context_.Reset();
}

void V8Context::Initialize(JSVM* vm, Runtime* runtime) {
    DLOG(INFO) << "V8CContext Initialize";
    JSContext::Initialize(vm, runtime);
    v8::Isolate* isolate = GetVM<v8::Isolate*>();
    v8::Isolate::Scope isolate_scope(isolate);
    v8::HandleScope handle_scope(isolate);

    // v8::V8::SetFlagsFromString(Constants::V8_STARTUP_FLAGS.c_str(),
    //                       Constants::V8_STARTUP_FLAGS.size());
    //const char* flag = "--expose_gc";
    //v8::V8::SetFlagsFromString(flag, static_cast<int>(strlen(flag)));

    v8::V8::SetCaptureStackTraceForUncaughtExceptions(true, 100, v8::StackTrace::kOverview);
    v8::V8::AddMessageListener(V8Context::OnUncaughtError);

    global_ = lynx_new Global(this);
    auto global_template = static_cast<V8PrototypeBuilder*>(
            global_->class_template()->prototype_builder())->GetClass(isolate)->InstanceTemplate();
    auto context = v8::Context::New(isolate, nullptr, global_template);
    v8::Context::Scope context_scope(context);
    context_.Reset(isolate, context);
    auto global_object = context->Global();
    V8ObjectWrap::Wrap(this, global_, global_object);
    global_object->Set(context, V8Helper::ConvertToV8String(isolate, "global"),
                       global_object).FromJust();
    global_object->Set(context, V8Helper::ConvertToV8String(isolate, "window"),
                       global_object).FromJust();
}

void V8Context::Clear() {
    v8::Isolate* isolate = GetVM<v8::Isolate*>();
    v8::Isolate::Scope isolate_scope(isolate);
    v8::HandleScope handleScope(isolate);
    v8::Local<v8::Context> context = GetContext();
    v8::Context::Scope context_scope(context);

    v8::Local<v8::Object> global = context->Global();
    v8::Local<v8::Array> keys = global->GetPropertyNames(context).ToLocalChecked();
    for (int i = 0; i < keys->Length(); i++) {
        auto key = keys->Get(context, i).ToLocalChecked();
        global->Set(key, v8::Null(isolate));
        LOGD("DELETE", "KEY = %s", V8Helper::ConvertToString(key->ToString()).c_str());
        //global->Delete(context, key);
    }
    isolate->LowMemoryNotification();
}

void V8Context::LoadUrl(const std::string &url) {
}

std::string V8Context::RunScript(const char* source) {

    v8::Isolate* isolate = GetVM<v8::Isolate*>();
    v8::Isolate::Scope isolate_scope(isolate);
    v8::HandleScope handleScope(isolate);
    v8::Local<v8::Context> context = GetContext();
    v8::Context::Scope context_scope(context);

    v8::TryCatch tc(isolate);

    v8::MaybeLocal<v8::Script> maybe_local_script = v8::Script::Compile(context,
            v8::String::NewFromUtf8(isolate, source, v8::NewStringType::kNormal).ToLocalChecked());
    v8::Local <v8::Script> script;
    if (!maybe_local_script.IsEmpty()) {
        script = maybe_local_script.ToLocalChecked();
    }

    if (tc.HasCaught()) {
        std::string error = GetErrorMessage(tc.Message(), tc.Exception());
        DLOG(ERROR) << error;
    }

    if (!script.IsEmpty()) {

        auto result = script->Run(context);

        if (result.IsEmpty()) {
            //printf("JSRuntime empty result");
        }

        if (tc.HasCaught()) {
            std::string error = GetErrorMessage(tc.Message(), tc.Exception());
            DLOG(ERROR) << error;
        }

    }

    return "";

}

void V8Context::AddJavaScriptInterface(const std::string &name,
                                       base::ScopedPtr<LynxObjectPlatform> object) {
    v8::Isolate* isolate = GetVM<v8::Isolate*>();
    v8::Isolate::Scope isolate_scope(isolate);
    v8::HandleScope handle_scope(isolate);
    v8::Local<v8::Context> context = GetContext();
    v8::Context::Scope context_scope(context);
    object->Attach(this);
    auto v8_object = V8Helper::ConvertToV8Object(isolate, object.Release());
    auto global = context->Global();
    global->Set(context, V8Helper::ConvertToV8String(isolate, name), v8_object).FromJust();
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
    DLOG(ERROR)<<m_message;
}
}  // namespace jscore
