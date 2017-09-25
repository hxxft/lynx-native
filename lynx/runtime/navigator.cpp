// Copyright 2017 The Lynx Authors. All rights reserved.
#include "runtime/base/lynx_value.h"
#include "runtime/navigator.h"

namespace jscore {
    Navigator::Navigator() {
        set_class_name("Navigator");
        RegisterAccessorCallback("userAgent", &GetUserAgentCallback, 0);
        RegisterAccessorCallback("appCodeName", &GetAppCodeNameCallback, 0);
        RegisterAccessorCallback("appName", &GetAppNameCallback, 0);
        RegisterAccessorCallback("platform", &GetPlatformCallback, 0);
        RegisterAccessorCallback("appVersion", &GetAppVersionCallback, 0);
    }
    
    Navigator::~Navigator() {

    }
    
    base::ScopedPtr<LynxValue> Navigator::GetUserAgentCallback(LynxObjectTemplate* object) {
        Navigator *navigator = static_cast<Navigator*>(object);
        LynxValue *value = LynxValue::MakeString(navigator->user_agent());
        return base::ScopedPtr<LynxValue>(value);
    }

    base::ScopedPtr<LynxValue> Navigator::GetAppCodeNameCallback(LynxObjectTemplate* object) {
        Navigator *navigator = static_cast<Navigator*>(object);
        LynxValue *value = LynxValue::MakeString(navigator->app_code_name());
        return base::ScopedPtr<LynxValue>(value);
    }

    base::ScopedPtr<LynxValue> Navigator::GetAppNameCallback(LynxObjectTemplate* object) {
        Navigator *navigator = static_cast<Navigator*>(object);
        LynxValue *value = LynxValue::MakeString(navigator->app_name());
        return base::ScopedPtr<LynxValue>(value);
    }

    base::ScopedPtr<LynxValue> Navigator::GetPlatformCallback(LynxObjectTemplate* object) {
        Navigator *navigator = static_cast<Navigator*>(object);
        LynxValue *value = LynxValue::MakeString(navigator->platform());
        return base::ScopedPtr<LynxValue>(value);
    }

    base::ScopedPtr<LynxValue> Navigator::GetAppVersionCallback(LynxObjectTemplate* object) {
        Navigator *navigator = static_cast<Navigator*>(object);
        LynxValue *value = LynxValue::MakeString(navigator->app_version());
        return base::ScopedPtr<LynxValue>(value);
    }
}