// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_RUNTIME_NAVIGATOR_H_
#define LYNX_RUNTIME_NAVIGATOR_H_

#include "runtime/config.h"
#include "runtime/base/lynx_object.h"

namespace jscore {
    class Navigator : public LynxObject {
    public:
        Navigator(JSContext* context);
        virtual ~Navigator();

        std::string user_agent();
        
        inline std::string app_code_name() {
            return APP_NAME;
        }
        
        inline std::string app_version() {
            return VERSION;
        }
        
        inline std::string platform() {
            return PLATFORM;
        }
        
        inline std::string app_name() {
            return APP_NAME;
        }

        base::ScopedPtr<LynxValue> GetUserAgent();
        base::ScopedPtr<LynxValue> GetAppCodeName();
        base::ScopedPtr<LynxValue> GetAppName();
        base::ScopedPtr<LynxValue> GetPlatform();
        base::ScopedPtr<LynxValue> GetAppVersion();

    };
}
#endif // LYNX_RUNTIME_NAVIGATOR_H_
