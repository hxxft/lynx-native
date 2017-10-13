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
        return LynxValue::MakeInt(width);
    }

    base::ScopedPtr<LynxValue> Screen::GetHeightCallback(LynxObjectTemplate* object) {
        int height = config::GlobalConfigData::GetInstance()->screen_height();
        return LynxValue::MakeInt(height);
    }
}