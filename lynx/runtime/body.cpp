// Copyright 2017 The Lynx Authors. All rights reserved.

#include "config/global_config_data.h"
#include "runtime/body.h"
#include "runtime/js_context.h"
#include "runtime/runtime.h"

namespace jscore {
    Body::Body(JSContext* context)
            : Element(context, context->runtime()->render_tree_host()->render_root()) {
        set_class_name("Body");

        RegisterAccessorCallback("clientWidth", &GetClientWidthCallback, 0);
        RegisterAccessorCallback("clientHeight", &GetClientHeightCallback, 0);
    }

    Body::~Body() {

    }

    base::ScopedPtr<LynxValue> Body::GetClientHeightCallback(LynxObjectTemplate *object) {
        int height = config::GlobalConfigData::GetInstance()->screen_height();
        return base::ScopedPtr<LynxValue>(LynxValue::MakeInt(height));
    }

    base::ScopedPtr<LynxValue> Body::GetClientWidthCallback(LynxObjectTemplate *object) {
        int width = config::GlobalConfigData::GetInstance()->screen_width();
        return base::ScopedPtr<LynxValue>(LynxValue::MakeInt(width));
    }

}