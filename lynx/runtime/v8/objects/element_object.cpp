// Copyright 2017 The Lynx Authors. All rights reserved.

#if USING_V8


#include <base/print.h>
#include "runtime/v8/objects/element_object.h"

#include "runtime/v8/v8_context.h"
#include "runtime/v8/v8_helper.h"
#include "runtime/v8/v8_js_function.h"
#include "config/global_config_data.h"

using namespace lynx;
using v8::Isolate;
using v8::FunctionTemplate;
using v8::Local;
using v8::External;
using v8::ObjectTemplate;
using v8::Value;
using v8::FunctionCallbackInfo;
using v8::Context;
using v8::PropertyCallbackInfo;

namespace {
static std::string kFlushStyle = "";
}


namespace jscore {

#define PixelToSP(value) (static_cast<int>(value / \
    config::GlobalConfigData::GetInstance()->screen_density()))

    static int ElementObjectCount = 0;

ElementObject::ElementObject(RenderObject* render_object): render_object_(render_object) {
    LOGD("lynx-debug", "construct element: %d", ++ElementObjectCount);
}

ElementObject::~ElementObject() {
    LOGD("lynx-debug", "destruct element: %d", --ElementObjectCount);
}

void ElementObject::SetFuncTemplate(Isolate* isolate, Local<FunctionTemplate> &func_tpl, Local<External> extData) {
    func_tpl->PrototypeTemplate()->Set(V8Helper::ConvertToV8String(isolate, "appendChild"), FunctionTemplate::New(isolate, AppendChildCallback));
    func_tpl->PrototypeTemplate()->Set(V8Helper::ConvertToV8String(isolate, "appendChildren"), FunctionTemplate::New(isolate, AppendChildrenCallback));
    func_tpl->PrototypeTemplate()->Set(V8Helper::ConvertToV8String(isolate, "insertChildAtIndex"), FunctionTemplate::New(isolate, InsertChildAtIndexCallback));
    func_tpl->PrototypeTemplate()->Set(V8Helper::ConvertToV8String(isolate, "removeChildByIndex"), FunctionTemplate::New(isolate, RemoveChildByIndexCallback));
    func_tpl->PrototypeTemplate()->Set(V8Helper::ConvertToV8String(isolate, "insertBefore"), FunctionTemplate::New(isolate, InsertBeforeCallback));
    func_tpl->PrototypeTemplate()->Set(V8Helper::ConvertToV8String(isolate, "removeChild"), FunctionTemplate::New(isolate, RemoveChildCallback));
    func_tpl->PrototypeTemplate()->Set(V8Helper::ConvertToV8String(isolate, "addEventListener"), FunctionTemplate::New(isolate, AddEventListenerCallback, extData));
    func_tpl->PrototypeTemplate()->Set(V8Helper::ConvertToV8String(isolate, "removeEventListener"), FunctionTemplate::New(isolate, RemoveEventListenerCallback, extData));
    func_tpl->PrototypeTemplate()->Set(V8Helper::ConvertToV8String(isolate, "setAttribution"), FunctionTemplate::New(isolate, SetAttributionCallback));
    func_tpl->PrototypeTemplate()->Set(V8Helper::ConvertToV8String(isolate, "setAttribute"), FunctionTemplate::New(isolate, SetAttributeCallback));
    func_tpl->PrototypeTemplate()->Set(V8Helper::ConvertToV8String(isolate, "setStyle"), FunctionTemplate::New(isolate, SetStyleCallback));
    func_tpl->PrototypeTemplate()->Set(V8Helper::ConvertToV8String(isolate, "setText"), FunctionTemplate::New(isolate, SetTextCallback));

    func_tpl->PrototypeTemplate()->SetAccessor(V8Helper::ConvertToV8String(isolate, "tagName"), GetTagNameCallback, NULL);
    func_tpl->PrototypeTemplate()->SetAccessor(V8Helper::ConvertToV8String(isolate, "nodeType"), GetNodeTypeCallback, NULL);
    func_tpl->PrototypeTemplate()->SetAccessor(V8Helper::ConvertToV8String(isolate, "parentNode"), GetParentNodeCallback, NULL);
    func_tpl->PrototypeTemplate()->SetAccessor(V8Helper::ConvertToV8String(isolate, "offsetTop"), GetOffsetTopCallback, NULL);
    func_tpl->PrototypeTemplate()->SetAccessor(V8Helper::ConvertToV8String(isolate, "offsetLeft"), GetOffsetLeftCallback, NULL);
    func_tpl->PrototypeTemplate()->SetAccessor(V8Helper::ConvertToV8String(isolate, "offsetWidth"), GetOffsetWidthCallback, NULL);
    func_tpl->PrototypeTemplate()->SetAccessor(V8Helper::ConvertToV8String(isolate, "offsetHeight"), GetOffsetHeightCallback, NULL);
    func_tpl->PrototypeTemplate()->SetAccessor(V8Helper::ConvertToV8String(isolate, "scrollWidth"), GetScrollWidthCallback, NULL);
    func_tpl->PrototypeTemplate()->SetAccessor(V8Helper::ConvertToV8String(isolate, "scrollHeight"), GetScrollHeightCallback, NULL);
    func_tpl->PrototypeTemplate()->SetAccessor(V8Helper::ConvertToV8String(isolate, "scrollTop"), GetScrollTopCallback, SetScrollTopCallback);
    func_tpl->PrototypeTemplate()->SetAccessor(V8Helper::ConvertToV8String(isolate, "scrollLeft"), GetScrollLeftCallback, SetScrollLeftCallback);
    func_tpl->PrototypeTemplate()->SetAccessor(V8Helper::ConvertToV8String(isolate, "forceScrollAnimate"), NULL, SetForceScrollAnimateCallback);

    func_tpl->PrototypeTemplate()->Set(V8Helper::ConvertToV8String(isolate, "start"), FunctionTemplate::New(isolate, StartCallback));
    func_tpl->PrototypeTemplate()->Set(V8Helper::ConvertToV8String(isolate, "stop"), FunctionTemplate::New(isolate, StopCallback));
    func_tpl->PrototypeTemplate()->Set(V8Helper::ConvertToV8String(isolate, "setPullView"), FunctionTemplate::New(isolate, SetPullViewCallback));
    func_tpl->PrototypeTemplate()->Set(V8Helper::ConvertToV8String(isolate, "closePullView"), FunctionTemplate::New(isolate, ClosePullViewCallback));
    func_tpl->PrototypeTemplate()->Set(V8Helper::ConvertToV8String(isolate, "getChildByIndex"), FunctionTemplate::New(isolate, GetChildByIndexCallback));
    func_tpl->PrototypeTemplate()->Set(V8Helper::ConvertToV8String(isolate, "getText"), FunctionTemplate::New(isolate, GetTextCallback));

    func_tpl->PrototypeTemplate()->Set(V8Helper::ConvertToV8String(isolate, "stopAnimate"), FunctionTemplate::New(isolate, StopAnimateCallback));
    func_tpl->PrototypeTemplate()->Set(V8Helper::ConvertToV8String(isolate, "startAnimateWithCallback"), FunctionTemplate::New(isolate, StartAnimateWithCallbackCallback));

    func_tpl->PrototypeTemplate()->SetAccessor(V8Helper::ConvertToV8String(isolate, "nextSibling"), GetNextSiblingCallback, NULL);
    func_tpl->PrototypeTemplate()->SetAccessor(V8Helper::ConvertToV8String(isolate, "index"), GetIndexCallback, NULL);
}

void ElementObject::BindingClass(Isolate* isolate, Local<ObjectTemplate> &globalObjectTemplate, V8Context* context, v8::Persistent<FunctionTemplate>& constructor) {

    // 创建js element对象回调
    auto extendData = External::New(isolate, context);
    auto tpl = FunctionTemplate::New(isolate, NewElementCallback);
    tpl->SetClassName(V8Helper::ConvertToV8String(isolate, "Element"));
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    // 增加元素js方法
    SetFuncTemplate(isolate, tpl, extendData);

    // 向js global中注册此元素
    globalObjectTemplate->Set(V8Helper::ConvertToV8String(isolate, "Element"), tpl);

    constructor.Reset(isolate, tpl);
}

// 初始化元素
void ElementObject::NewElementCallback(const FunctionCallbackInfo<Value>& args) {

    Isolate* isolate = args.GetIsolate();
    Local<Context> context = isolate->GetCurrentContext();
    if (args.IsConstructCall()) {
        if (args.Length() ==  0) {
            //do not support
            ElementObject* obj = lynx_new ElementObject(nullptr);
            obj->Wrap(args.This());
            args.GetReturnValue().Set(args.This());
        } else {
            // 创建c对象与v8对象和java对象建立关系
            RenderObject* render_object = reinterpret_cast<RenderObject*>(args[0]->IntegerValue(context).FromJust());
            ElementObject* obj = lynx_new ElementObject(render_object);
            render_object->SetJSRef(obj);
            obj->Wrap(args.This());
            args.GetReturnValue().Set(args.This());
        }
    }
}

// 创建js对象
Local<Value> ElementObject::CreateElement(Isolate* isolate, RenderObject* render_object, v8::Persistent<FunctionTemplate>& constructor) {
    Local<Context> context = isolate->GetCurrentContext();
    Local<FunctionTemplate> cons_tpl = Local<FunctionTemplate>::New(isolate, constructor);
    // 调用v8的对象构造器,传入javaobject对象
    auto cons = cons_tpl->GetFunction(context).ToLocalChecked();
    const int argc = 1;
    Local<Value> argv[argc] = { v8::Number::New(isolate, reinterpret_cast<long>(render_object)) };
    Local<v8::Object> result = cons->NewInstance(context, argc, argv).ToLocalChecked();
    return result;
}

void ElementObject::AppendChildCallback(const FunctionCallbackInfo<Value>& args) {

    Isolate* isolate = args.GetIsolate();
    Local<Context> context = isolate->GetCurrentContext();
    ElementObject* obj = ObjectWrap::Unwrap<ElementObject>(args.This());
    Local<v8::Value> value = args[0];
    if (value->IsNullOrUndefined())
        return;
    ElementObject* child = ObjectWrap::Unwrap<ElementObject>(value->ToObject(context).ToLocalChecked());
    //child->Ref();
    if (obj && (obj->render_object_.Get() != NULL) && child && (child->render_object_.Get() != NULL)) {
        obj->render_object_->AppendChild(child->render_object_.Get());
    }
}

void ElementObject::AppendChildrenCallback(const FunctionCallbackInfo<Value>& args) {

    Isolate* isolate = args.GetIsolate();
    Local<Context> context = isolate->GetCurrentContext();
    ElementObject* obj = ObjectWrap::Unwrap<ElementObject>(args.Holder());
    auto children = Local<v8::Array>::Cast(args[0]);
    int len = children->Length();
    for (int i = 0; i < len; i++) {
        ElementObject* child = ObjectWrap::Unwrap<ElementObject>(
                                   children->Get(context, i).ToLocalChecked()->ToObject(context).ToLocalChecked());
        if (obj && child) {
            obj->render_object_->AppendChild(child->render_object_.Get());
            //child->Ref();
        }
    }
}

void ElementObject::InsertChildAtIndexCallback(const FunctionCallbackInfo<Value>& args) {
    if (args.Length() < 2) {
        return;
    }

    Isolate* isolate = args.GetIsolate();
    Local<Context> context = isolate->GetCurrentContext();
    ElementObject* obj = ObjectWrap::Unwrap<ElementObject>(args.Holder());
    ElementObject* child = ObjectWrap::Unwrap<ElementObject>(args[0]->ToObject(context).ToLocalChecked());
    if (obj && child) {
        obj->render_object_->InsertChild(child->render_object_.Get(), args[1]->Int32Value(context).FromJust());
        //child->Ref();
    }
}

void ElementObject::RemoveChildByIndexCallback(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();
    Local<Context> context = isolate->GetCurrentContext();
    ElementObject* obj = ObjectWrap::Unwrap<ElementObject>(args.Holder());
    args.Holder()->Delete(args[0]);
    if (obj && (obj->render_object_.Get() != NULL)) {
        //need unref
        RenderObject* render_object = obj->render_object_->RemoveChildByIndex(args[0]->Int32Value(context).FromJust());
        //if (render_object->GetJSRef() != NULL) {
        //    reinterpret_cast<ElementObject*>(render_object->GetJSRef())->Unref();
        //}
    }
}

void ElementObject::InsertBeforeCallback(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();
    Local<Context> context = isolate->GetCurrentContext();
    ElementObject* obj = ObjectWrap::Unwrap<ElementObject>(args.Holder());
    ElementObject* new_node = ObjectWrap::Unwrap<ElementObject>(args[0]->ToObject(context).ToLocalChecked());
    //new_node->Ref();

    if (args.Length() > 1 && !args[1]->IsNull() && !args[1]->IsUndefined() ) {
        ElementObject* reference_node = ObjectWrap::Unwrap<ElementObject>(args[1]->ToObject(context).ToLocalChecked());
        if (obj && (obj->render_object_.Get() != NULL) && new_node && reference_node) {
            obj->render_object_->InsertBefore(new_node->render_object_.Get(), reference_node->render_object_.Get());
        }
    } else {

        ElementObject* new_node = ObjectWrap::Unwrap<ElementObject>(args[0]->ToObject(context).ToLocalChecked());
        if (obj && new_node) {
            obj->render_object_->AppendChild(new_node->render_object_.Get());
        }
    }
}

void ElementObject::RemoveChildCallback(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();
    Local<Context> context = isolate->GetCurrentContext();
    ElementObject* obj = ObjectWrap::Unwrap<ElementObject>(args.Holder());
    ElementObject* child = ObjectWrap::Unwrap<ElementObject>(args[0]->ToObject(context).ToLocalChecked());
    args.Holder()->Delete(args[0]);
    if (obj && child) {
        obj->render_object_->RemoveChild(child->render_object_.Get());
        //child->Unref();
    }
}

void ElementObject::AddEventListenerCallback(const FunctionCallbackInfo<Value>& args) {

    Isolate* isolate = args.GetIsolate();
    ElementObject* obj = ObjectWrap::Unwrap<ElementObject>(args.Holder());

    std::string event = V8Helper::ConvertToString(args[0]->ToString());
    V8Context* context = reinterpret_cast<V8Context*>(args.Data().As<v8::External>()->Value());
    if (obj) {
        //  base::ScopedPtr<JSFunction> function(new V8JSFunction(runtime, args.This(), args[1]));
        obj->render_object_->AddEventListener(event, lynx_new V8JSFunction(context, args.This(), args[1]));
    }

}

void ElementObject::RemoveEventListenerCallback(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();
    ElementObject* obj = ObjectWrap::Unwrap<ElementObject>(args.Holder());

    std::string event = V8Helper::ConvertToString(args[0]->ToString());
    V8Context* context = reinterpret_cast<V8Context*>(args.Data().As<v8::External>()->Value());
    if (obj) {
        //  base::ScopedPtr<JSFunction> function(new V8JSFunction(runtime, args.This(), args[1]));
        obj->render_object_->RemoveEventListener(event, lynx_new V8JSFunction(context, args.This(), args[1]));
    }
}

void ElementObject::SetAttributionCallback(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();
    Local<Context> context = isolate->GetCurrentContext();
    ElementObject* obj = ObjectWrap::Unwrap<ElementObject>(args.Holder());
    if (obj && args[0]->IsObject()) {
        Local<v8::Object> map = Local<v8::Object>::Cast(args[0]);
        auto v8_keys_maybe = map->GetPropertyNames(context);
        if (v8_keys_maybe.IsEmpty()) return;
        Local<v8::Array> v8_keys = v8_keys_maybe.ToLocalChecked();
        int length = v8_keys->Length();
        for (int i = 0; i < length; ++i) {
            auto v8_key = v8_keys->Get(i);
            auto v8_value_maybe = map->Get(context, v8_key);
            Local<Value> v8_value;
            if (!v8_value_maybe.IsEmpty()) {
                v8_value = v8_value_maybe.ToLocalChecked();
            } else {
                v8_value = Local<Value>(Null(isolate));
            }
            std::string key = V8Helper::ConvertToString(v8_key->ToString(context).ToLocalChecked());
            std::string value = V8Helper::ConvertToString(v8_value->ToString(context).ToLocalChecked());
            obj->render_object_->SetAttribute(key, value);
        }
    }
}

void ElementObject::SetAttributeCallback(const FunctionCallbackInfo<Value>& args) {

    Isolate* isolate = args.GetIsolate();
    Local<Context> context = isolate->GetCurrentContext();
    ElementObject* obj = ObjectWrap::Unwrap<ElementObject>(args.Holder());
    std::string key = V8Helper::ConvertToString(args[0]->ToString(context).ToLocalChecked());
    std::string  value = V8Helper::ConvertToString(args[1]->ToString(context).ToLocalChecked());
    if (obj) {
        obj->render_object_->SetAttribute(key, value);
    }
}

void ElementObject::SetStyleCallback(const FunctionCallbackInfo<Value>& args) {

    ElementObject* obj = ObjectWrap::Unwrap<ElementObject>(args.Holder());
    if (obj == NULL || obj->render_object_.Get() == NULL) {
        return;
    }

    Isolate* isolate = args.GetIsolate();
    Local<Context> context = isolate->GetCurrentContext();
    Local<v8::Object> styles =  Local<v8::Object>::Cast(args[0]);

    Local<v8::Array> names = styles->GetOwnPropertyNames(context).ToLocalChecked();
    int len = names->Length();
    for (int i = 0; i < len; i++) {
        Local<Value> name = names->Get(context, v8::Integer::New(isolate, i)).ToLocalChecked();
        std::string name_str = V8Helper::ConvertToString(Local<v8::String>::Cast(name));

        Local<Value> value = styles->Get(context, name).ToLocalChecked();

        if (value->IsString()) {
            std::string value_str = V8Helper::ConvertToString(Local<v8::String>::Cast(value));
            obj->render_object_->SetStyle(name_str, value_str);
        } else if (value->IsNumber()) {
            std::string value_str = V8Helper::ConvertToString(value->ToString(context).ToLocalChecked());
            obj->render_object_->SetStyle(name_str, value_str);
        }
        obj->render_object_->SetStyle(kFlushStyle, kFlushStyle);
    }
}

void ElementObject::SetTextCallback(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();
    Local<Context> context = isolate->GetCurrentContext();
    ElementObject* obj = ObjectWrap::Unwrap<ElementObject>(args.Holder());
    std::string text = V8Helper::ConvertToString(args[0]->ToString(context).ToLocalChecked());
    if (obj && (obj->render_object_.Get() != NULL)) {
        obj->render_object_->SetText(text);
    }
}

void ElementObject::GetTextCallback(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();
    ElementObject* obj = ObjectWrap::Unwrap<ElementObject>(args.Holder());
    std::string result = obj && (obj->render_object_.Get() != NULL) ? obj->render_object_->GetText() : "";
    args.GetReturnValue().Set(V8Helper::ConvertToV8String(isolate, result));
}


void ElementObject::StopAnimateCallback(const v8::FunctionCallbackInfo <v8::Value> &args) {

}

void ElementObject::StartAnimateWithCallbackCallback(const v8::FunctionCallbackInfo <v8::Value> &args) {

}

void ElementObject::GetNodeTypeCallback(Local<v8::String> property, const PropertyCallbackInfo<Value>& info) {
    Isolate* isolate = info.GetIsolate();
    info.GetReturnValue().Set(v8::Number::New(isolate, 1));
}

void ElementObject::GetParentNodeCallback(Local<v8::String> property, const PropertyCallbackInfo<Value>& info) {
    Isolate* isolate = info.GetIsolate();
    ElementObject* obj = ObjectWrap::Unwrap<ElementObject>(info.This());
    if ((obj->render_object_.Get() != NULL) && obj->render_object_->Parent() != NULL) {
        RenderObject* parent = const_cast<RenderObject*>(obj->render_object_->Parent());
        ElementObject* parentObj = reinterpret_cast<ElementObject*>(parent->GetTarget());
        if (parentObj != NULL || parent->GetJSRef() != NULL) {
            info.GetReturnValue().Set(Local<v8::Object>::New(isolate, parentObj->persistent()));
        } else {
            info.GetReturnValue().SetUndefined();
        }
    } else {
        info.GetReturnValue().SetUndefined();
    }
}

void ElementObject::GetTagNameCallback(Local<v8::String> property, const PropertyCallbackInfo<Value>& info) {
    ElementObject* obj = ObjectWrap::Unwrap<ElementObject>(info.This());
    std::string result = obj && (obj->render_object_.Get() != NULL) ? obj->render_object_->tag_name() : "";
    info.GetReturnValue().Set(V8Helper::ConvertToV8String(info.GetIsolate(), result));
}


void ElementObject::GetOffsetTopCallback(Local<v8::String> property, const PropertyCallbackInfo<Value>& info) {

    Isolate* isolate = info.GetIsolate();
    ElementObject* obj = ObjectWrap::Unwrap<ElementObject>(info.This());
    int offsetTop = obj && (obj->render_object_.Get() != NULL) ? PixelToSP(obj->render_object_->offset_top()) : 0;
    info.GetReturnValue().Set(v8::Number::New(isolate, offsetTop));

}

void ElementObject::GetOffsetLeftCallback(Local<v8::String> property, const PropertyCallbackInfo<Value>& info) {
    Isolate* isolate = info.GetIsolate();
    ElementObject* obj = ObjectWrap::Unwrap<ElementObject>(info.This());
    int offsetLeft = obj && (obj->render_object_.Get() != NULL) ? PixelToSP(obj->render_object_->offset_left()) : 0;
    info.GetReturnValue().Set(v8::Number::New(isolate, offsetLeft));
}

void ElementObject::GetOffsetWidthCallback(Local<v8::String> property, const PropertyCallbackInfo<Value>& info) {
    Isolate* isolate = info.GetIsolate();
    ElementObject* obj = ObjectWrap::Unwrap<ElementObject>(info.This());
    int offsetWidth = obj && (obj->render_object_.Get() != NULL) ? PixelToSP(obj->render_object_->offset_width()) : 0;
    info.GetReturnValue().Set(v8::Number::New(isolate, offsetWidth));
}

void ElementObject::GetOffsetHeightCallback(Local<v8::String> property, const PropertyCallbackInfo<Value>& info) {
    Isolate* isolate = info.GetIsolate();
    ElementObject* obj = ObjectWrap::Unwrap<ElementObject>(info.This());
    int offsetHeight = obj && (obj->render_object_.Get() != NULL) ? PixelToSP(obj->render_object_->offset_height()) : 0;
    info.GetReturnValue().Set(v8::Number::New(isolate, offsetHeight));
}

void ElementObject::GetScrollWidthCallback(Local<v8::String> property, const PropertyCallbackInfo<Value>& info) {
    Isolate* isolate = info.GetIsolate();
    ElementObject* obj = ObjectWrap::Unwrap<ElementObject>(info.This());
    int scrollWidth = obj && (obj->render_object_.Get() != NULL) ? PixelToSP(obj->render_object_->scroll_width()) : 0;
    info.GetReturnValue().Set(v8::Number::New(isolate, scrollWidth));
}

void ElementObject::GetScrollHeightCallback(Local<v8::String> property, const PropertyCallbackInfo<Value>& info) {
    Isolate* isolate = info.GetIsolate();
    ElementObject* obj = ObjectWrap::Unwrap<ElementObject>(info.This());
    int scrollHeight = obj && (obj->render_object_.Get() != NULL) ? PixelToSP(obj->render_object_->scroll_height()) : 0;
    info.GetReturnValue().Set(v8::Number::New(isolate, scrollHeight));
}

void ElementObject::SetScrollTopCallback(Local<v8::String> property, Local<Value> value, const PropertyCallbackInfo<void>& info) {
    Isolate* isolate = info.GetIsolate();
    ElementObject* obj = ObjectWrap::Unwrap<ElementObject>(info.This());
    int scroll_top = value->Int32Value();
    if (obj && (obj->render_object_.Get() != NULL)) {
        obj->render_object_->SetScrollTop(scroll_top);
    }
}

void ElementObject::GetScrollTopCallback(Local<v8::String> property, const PropertyCallbackInfo<Value>& info) {

    Isolate* isolate = info.GetIsolate();
    ElementObject* obj = ObjectWrap::Unwrap<ElementObject>(info.This());
    int scrollTop = obj && (obj->render_object_.Get() != NULL) ? PixelToSP(obj->render_object_->scroll_top()) : 0;
    info.GetReturnValue().Set(v8::Number::New(isolate, scrollTop));
}

void ElementObject::SetScrollLeftCallback(Local<v8::String> property, Local<Value> value, const PropertyCallbackInfo<void>& info) {
    Isolate* isolate = info.GetIsolate();
    Local<Context> context = isolate->GetCurrentContext();
    ElementObject* obj = ObjectWrap::Unwrap<ElementObject>(info.This());
    auto maybe_scroll_left = value->NumberValue(context);
    if (maybe_scroll_left.IsJust() && obj && (obj->render_object_.Get() != NULL)) {
        int scroll_left = (int) maybe_scroll_left.ToChecked();
        obj->render_object_->SetScrollLeft(scroll_left);
    }
}

void ElementObject::GetScrollLeftCallback(Local<v8::String> property, const PropertyCallbackInfo<Value>& info) {
    Isolate* isolate = info.GetIsolate();
    ElementObject* obj = ObjectWrap::Unwrap<ElementObject>(info.This());
    int scrollLeft = obj && (obj->render_object_.Get() != NULL) ? PixelToSP(obj->render_object_->scroll_left()) : 0;
    info.GetReturnValue().Set(v8::Number::New(isolate, scrollLeft));
}


void ElementObject::GetNextSiblingCallback(Local<v8::String> property, const PropertyCallbackInfo<Value>& info) {
    Isolate* isolate = info.GetIsolate();
    ElementObject* obj = ObjectWrap::Unwrap<ElementObject>(info.This());
    if ((obj->render_object_.Get() != NULL) && obj->render_object_->Next() != NULL) {
        RenderObject* next = obj->render_object_->NextSibling();
        ElementObject* next_obj = reinterpret_cast<ElementObject*>(next->GetJSRef());
        if (next_obj != NULL && next->GetJSRef() != NULL) {
            info.GetReturnValue().Set(Local<v8::Object>::New(isolate, next_obj->persistent()));
        } else {
            info.GetReturnValue().SetUndefined();
        }
    } else {
        info.GetReturnValue().SetUndefined();
    }
}

void ElementObject::GetIndexCallback(Local<v8::String> property, const PropertyCallbackInfo<Value>& info) {

}

void ElementObject::SetForceScrollAnimateCallback(Local<v8::String> property, Local<Value> value, const PropertyCallbackInfo<void>& info) {
    /*
    Isolate* isolate = info.GetIsolate();
    ElementObject* obj = ObjectWrap::Unwrap<ElementObject>(info.This());
    jEnv.CallVoidMethod(obj->java_object(), set_force_scroll_animate_method_, value->BooleanValue());
     */
}

void ElementObject::StartCallback(const v8::FunctionCallbackInfo <v8::Value> &args) {
    /*
    Isolate* isolate = args.GetIsolate();
    ElementObject* obj = ObjectWrap::Unwrap<ElementObject>(args.Holder());
    jEnv.CallVoidMethod(obj->java_object(), start_method_);
     */
}

void ElementObject::StopCallback(const v8::FunctionCallbackInfo <v8::Value> &args) {
    /*
    Isolate* isolate = args.GetIsolate();
    ElementObject* obj = ObjectWrap::Unwrap<ElementObject>(args.Holder());
    jEnv.CallVoidMethod(obj->java_object(), stop_method_, args[0]->Int32Value());
     */
}

void ElementObject::SetPullViewCallback(const v8::FunctionCallbackInfo <v8::Value> &args) {
    /*
    Isolate* isolate = args.GetIsolate();
    ElementObject* obj = ObjectWrap::Unwrap<ElementObject>(args.Holder());
    ElementObject* element = ObjectWrap::Unwrap<ElementObject>(args[0]->ToObject());
    jEnv.CallVoidMethod(obj->java_object(), set_pull_view_method_, element->java_object());
     */
}

void ElementObject::ClosePullViewCallback(const v8::FunctionCallbackInfo <v8::Value> &args) {
    /*
    Isolate* isolate = args.GetIsolate();
    ElementObject* obj = ObjectWrap::Unwrap<ElementObject>(args.Holder());
    jEnv.CallVoidMethod(obj->java_object(), close_pull_view_method_);
     */
}

void ElementObject::GetChildByIndexCallback(const v8::FunctionCallbackInfo <v8::Value> &args) {
    Isolate* isolate = args.GetIsolate();
    ElementObject* obj = ObjectWrap::Unwrap<ElementObject>(args.Holder());
    int index = args[0]->Int32Value();
    if (obj == NULL || obj->render_object_.Get() == NULL) {
        args.GetReturnValue().SetUndefined();
        return;
    }
    RenderObject* render_object_ = const_cast<RenderObject*>(obj->render_object_->Get(index));
    if (render_object_ != 0 && render_object_->GetJSRef() != NULL) {
        ElementObject* child = reinterpret_cast<ElementObject*>(render_object_->GetJSRef());
        args.GetReturnValue().Set(Local<v8::Object>::New(isolate, child->persistent()));
    } else {
        args.GetReturnValue().SetUndefined();
    }
}

}
#endif
