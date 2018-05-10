// Copyright 2017 The Lynx Authors. All rights reserved.

#include "runtime/base/lynx_object_platform.h"
#include "runtime/js/js_context.h"

namespace jscore {

    void LynxObjectPlatform::Attach(JSContext *context) {
        context_ = context;
        set_class_template(context_->class_template_storage()->GetEmptyClassTemplate(context));
    }
}