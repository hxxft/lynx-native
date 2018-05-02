// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_LOADER_SCRIPT_SCRIPT_REQUEST_DELEGATE_H_
#define LYNX_LOADER_SCRIPT_SCRIPT_REQUEST_DELEGATE_H_

#include "net/url_request.h"

namespace loader {
class ScriptLoader;
class ScriptRequestDelegate: public net::URLRequestDelegate {
 public:
    ScriptRequestDelegate(ScriptLoader* loader, const std::string& url);
    ~ScriptRequestDelegate();
    virtual void OnSuccess(base::ScopedPtr<base::PlatformString> url,
                           base::ScopedPtr<base::PlatformString> response);
    virtual void OnFailed(base::ScopedPtr<base::PlatformString> url,
                          base::ScopedPtr<base::PlatformString> error);

 private:
    ScriptLoader* loader_;
    std::string url_;
};
}  // namespace loader

#endif  // LYNX_LOADER_SCRIPT_SCRIPT_REQUEST_DELEGATE_H_
