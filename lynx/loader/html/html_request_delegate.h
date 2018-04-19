// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_LOADER_HTML_HTML_FILE_REQUEST_DELEGATE_H_
#define LYNX_LOADER_HTML_HTML_FILE_REQUEST_DELEGATE_H_

#include "net/url_request.h"
#include "loader/loader.h"
#include "loader/html/html_loader.h"

namespace loader {
class HTMLRequestDelegate: public net::URLRequestDelegate {
 public:
    HTMLRequestDelegate(HTMLLoader* loader, const std::string& url);
    ~HTMLRequestDelegate();
    virtual void OnSuccess(base::ScopedPtr<base::PlatformString> url,
                           base::ScopedPtr<base::PlatformString> response);
    virtual void OnFailed(base::ScopedPtr<base::PlatformString> url,
                          base::ScopedPtr<base::PlatformString> error);

 private:
    HTMLLoader* loader_;
    std::string url_;
};
}  // namespace loader

#endif  // LYNX_LOADER_HTML_HTML_FILE_REQUEST_DELEGATE_H_
