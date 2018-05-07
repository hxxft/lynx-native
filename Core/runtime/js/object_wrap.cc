// Copyright 2017 The Lynx Authors. All rights reserved.
#include "runtime/js/object_wrap.h"
#include "runtime/js/js_context.h"

namespace jscore {
    ObjectWrap::ObjectWrap(JSContext* context, LynxObject* obj) :
            context_(context), lynx_object_(obj) {
        context_->AddObserver(this);
    }

    ObjectWrap::~ObjectWrap() {
        context_->RemoveObserver(this);
    }
}