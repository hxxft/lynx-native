// Copyright 2017 The Lynx Authors. All rights reserved.

#include "runtime/js_context.h"
#include "runtime/location.h"
#include "runtime/history.h"
#include "runtime/navigator.h"

namespace jscore {

    std::string JSContext::GetPageUrl() {
        return location_->href();
    }
}