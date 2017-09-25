// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_RUNTIME_NAVIGATOR_H_
#define LYNX_RUNTIME_NAVIGATOR_H_

#include "runtime/config.h"
#include "runtime/base/lynx_object_template.h"

namespace jscore {
    class Navigator : public LynxObjectTemplate {
    public:
        Navigator();
        virtual ~Navigator();

        inline std::string user_agent() {
            return USERAGENT;
        }
        
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

    private:

        static base::ScopedPtr<LynxValue> GetUserAgentCallback(LynxObjectTemplate* object);
        static base::ScopedPtr<LynxValue> GetAppCodeNameCallback(LynxObjectTemplate* object);
        static base::ScopedPtr<LynxValue> GetAppNameCallback(LynxObjectTemplate* object);
        static base::ScopedPtr<LynxValue> GetPlatformCallback(LynxObjectTemplate* object);
        static base::ScopedPtr<LynxValue> GetAppVersionCallback(LynxObjectTemplate* object);
    };
}
#endif // LYNX_RUNTIME_NAVIGATOR_H_
