// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_RUNTIME_V8_PROTOTYPE_BUILDER_H_
#define LYNX_RUNTIME_V8_PROTOTYPE_BUILDER_H_

#include "v8.h"
#include "runtime/js/class_template.h"
#include "runtime/v8/object_wrap.h"
#include "runtime/v8/v8_helper.h"

namespace jscore {
    class V8PrototypeBuilder : public ClassTemplate::PrototypeBuilder {
    public:

        V8PrototypeBuilder() {
        }

        ~V8PrototypeBuilder() {
            class_tpl_.Reset();
        }

        v8::Local<v8::FunctionTemplate> GetClass(v8::Isolate* isolate) {
            return class_tpl_.Get(isolate);
        }

        v8::Local<v8::Object> MakeObject(v8::Local<v8::Context> context);

        void FinishCreation(ClassTemplate* class_template);

    private:
        v8::Persistent<v8::FunctionTemplate> class_tpl_;
    };
}

#endif //LYNX_RUNTIME_V8_PROTOTYPE_BUILDER_H_