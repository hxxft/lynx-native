// Copyright 2017 The Lynx Authors. All rights reserved.

#include "runtime/screen.h"
#include "config/global_config_data.h"
#include "runtime/base/lynx_value.h"
#include "runtime/js/js_context.h"
#include "runtime/js/class_template.h"

namespace jscore {

    #define FOR_EACH_FIELD_BINDING(V)   \
        V(Screen, Width)                \
        V(Screen, Height)

    // Defines methods and fields
    FOR_EACH_FIELD_BINDING(DEFINE_GET_CALLBACK)

    // Defines default ClassTemplate
    DEFINE_CLASS_TEMPLATE_START(Screen)
        FOR_EACH_FIELD_BINDING(REGISTER_GET_CALLBACK)
    DEFINE_CLASS_TEMPLATE_END

    Screen::Screen(JSContext* context) : LynxObject(context, DEFAULT_CLASS_TEMPLATE(context)) {
    }

    Screen::~Screen() {

    }

    base::ScopedPtr<LynxValue> Screen::GetWidth() {
        int width = config::GlobalConfigData::GetInstance()->screen_width();
        return LynxValue::MakeInt(width);
    }

    base::ScopedPtr<LynxValue> Screen::GetHeight() {
        int height = config::GlobalConfigData::GetInstance()->screen_height();
        return LynxValue::MakeInt(height);
    }
}