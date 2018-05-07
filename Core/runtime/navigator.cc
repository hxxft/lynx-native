// Copyright 2017 The Lynx Authors. All rights reserved.
#include "runtime/navigator.h"

#include "runtime/base/lynx_value.h"
#include "runtime/js/class_template.h"
#include "runtime/js/js_context.h"

namespace jscore {

    #define FOR_EACH_FIELD_BINDING(V)   \
        V(Navigator, UserAgent)         \
        V(Navigator, AppCodeName)       \
        V(Navigator, AppName)           \
        V(Navigator, Platform)          \
        V(Navigator, AppVersion)

    // Defines methods and fields
    FOR_EACH_FIELD_BINDING(DEFINE_GET_CALLBACK)

    // Defines default ClassTemplate
    DEFINE_CLASS_TEMPLATE_START(Navigator)
        FOR_EACH_FIELD_BINDING(REGISTER_GET_CALLBACK)
    DEFINE_CLASS_TEMPLATE_END

    Navigator::Navigator(JSContext* context) : LynxObject(context,
                                                          DEFAULT_CLASS_TEMPLATE(context)) {
    }
    
    Navigator::~Navigator() {

    }

    std::string Navigator::user_agent() {
        return context_->GetUserAgent();
    }
    
    base::ScopedPtr<LynxValue> Navigator::GetUserAgent() {
        return LynxValue::MakeString(user_agent());
    }

    base::ScopedPtr<LynxValue> Navigator::GetAppCodeName() {
        return LynxValue::MakeString(app_code_name());
    }

    base::ScopedPtr<LynxValue> Navigator::GetAppName() {
        return LynxValue::MakeString(app_name());
    }

    base::ScopedPtr<LynxValue> Navigator::GetPlatform() {
        return LynxValue::MakeString(platform());
    }

    base::ScopedPtr<LynxValue> Navigator::GetAppVersion() {
        return LynxValue::MakeString(app_version());
    }
}