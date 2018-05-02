// Copyright 2017 The Lynx Authors. All rights reserved.

#include "config/global_config_data.h"

#include <stddef.h>

#include "base/debug/memory_debug.h"
#include "layout/css_style.h"

namespace {
config::GlobalConfigData* g_global_config_data = NULL;
}

namespace config {

GlobalConfigData* GlobalConfigData::GetInstance() {
    if (g_global_config_data == NULL) {
        g_global_config_data = lynx_new GlobalConfigData();
        lynx::CSSStyle::Initialize(g_global_config_data->style_config());
    }
    return g_global_config_data;
}

}  // namespace config
