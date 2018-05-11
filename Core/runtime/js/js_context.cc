// Copyright 2017 The Lynx Authors. All rights reserved.

#include "runtime/js/js_context.h"
#include "runtime/runtime.h"
#include "runtime/global.h"

namespace jscore {

    JSContext::JSContext() : runtime_(NULL),
                             vm_(NULL),
                             ua_(),
                             class_template_storage_(lynx_new ClassTemplateStorage),
                             global_(NULL) {

    }

    JSContext::~JSContext() {
        Finalize();
    }

    void JSContext::OnExceptionOccurred(std::string &error) {
        if(runtime_ && runtime_->exception_handler()){
          runtime_->exception_handler()->OnReceiveResult(error);
        }
    }
}
