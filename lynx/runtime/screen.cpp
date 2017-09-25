// Copyright 2017 The Lynx Authors. All rights reserved.

#include "runtime/screen.h"
#include "config/global_config_data.h"
#include "runtime/base/lynx_value.h"

namespace jscore {

    Screen::Screen() {
        set_class_name("Screen");
        RegisterAccessorCallback("width", &GetWidthCallback, 0);
        RegisterAccessorCallback("height", &GetHeightCallback, 0);
    }

    Screen::~Screen() {

    }

    base::ScopedPtr<LynxValue> Screen::GetWidthCallback(LynxObjectTemplate* object) {
        int width = config::GlobalConfigData::GetInstance()->screen_width();
        LynxValue *value = LynxValue::MakeInt(width);
        return LynxValue::MakeValueScoped(value);
    }

    base::ScopedPtr<LynxValue> Screen::GetHeightCallback(LynxObjectTemplate* object) {
        int height = config::GlobalConfigData::GetInstance()->screen_height();
        LynxValue *value = LynxValue::MakeInt(height);
        return LynxValue::MakeValueScoped(value);
    }
}