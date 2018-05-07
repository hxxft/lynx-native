// Copyright 2017 The Lynx Authors. All rights reserved.
#include "runtime/v8/prototype_builder.h"
#include "runtime/v8/v8_context.h"

namespace jscore {

    ClassTemplate::PrototypeBuilder* ClassTemplate::CreatePrototypeBuilder() {
        return lynx_new V8PrototypeBuilder();
    }

    v8::Local<v8::Object> V8PrototypeBuilder::MakeObject(v8::Local<v8::Context> context) {
        auto cons = class_tpl_.Get(context->GetIsolate())->GetFunction(context).ToLocalChecked();
        v8::Local<v8::Object> result =
                cons->NewInstance(context, 0, nullptr).ToLocalChecked();
        return result;
    }

    void V8PrototypeBuilder::FinishCreation(ClassTemplate* class_template) {
        v8::Isolate* isolate = class_template->context()->GetVM<v8::Isolate*>();
        v8::HandleScope handle_scope(isolate);
        v8::Local<v8::FunctionTemplate> class_tpl = v8::FunctionTemplate::New(isolate);
        class_tpl->InstanceTemplate()->SetInternalFieldCount(1);

        v8::Local<v8::Context> context = static_cast<V8Context*>(class_template->context())->GetContext();

        bool expose_constructor = class_template->constructor_expose_enable();
        class_tpl->SetClassName(V8Helper::ConvertToV8String(isolate,
                                                            class_template->class_name()));
        if (class_template->parent_class_template() != NULL) {
            class_tpl->Inherit(static_cast<V8PrototypeBuilder*>(class_template
                    ->parent_class_template()->prototype_builder())->class_tpl_.Get(isolate));
        }

        auto& field_map = class_template->fields();
        for (auto& it : field_map) {
            Field field = it.second;
            auto get = field.get_callback;
            auto set = field.set_callback;
            if (field.get_callback == 0) get = 0;
            if (field.set_callback == 0) set = 0;
            class_tpl->PrototypeTemplate()->SetAccessor(
                    V8Helper::ConvertToV8String(isolate, it.first), get, set);
        }

        auto& method_map = class_template->methods();
        for (auto& it : method_map) {
            class_tpl->PrototypeTemplate()->Set(
                    V8Helper::ConvertToV8String(isolate, it.first),
                    v8::FunctionTemplate::New(isolate, it.second));
        }

        if (expose_constructor) {
            context->Global()->Set(V8Helper::ConvertToV8String(isolate, class_template->class_name()),
                                   class_tpl->GetFunction(context).ToLocalChecked());
        }

        class_tpl_.Reset(isolate, class_tpl);
    }
}