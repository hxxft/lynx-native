// Copyright 2017 The Lynx Authors. All rights reserved.

#include "runtime/js_context.h"
#include "runtime/location.h"
#include "runtime/runtime.h"

namespace jscore {

    void JSContext::OnExceptionOccured(std::string &error) {
        runtime_->exception_handler()->OnReceiveResult(error);
    }

    std::string JSContext::GetPageUrl() {
        return location_->href();
    }
}