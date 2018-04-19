// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_RUNTIME_RUNTIME_URL_REQUEST_DELEGATE_H_
#define LYNX_RUNTIME_RUNTIME_URL_REQUEST_DELEGATE_H_

#include "base/weak_ptr.h"
#include "runtime/runtime.h"
#include "net/url_request_delegate.h"

namespace jscore {
class RuntimeURLRequestDelegate : public net::URLRequestDelegate {
 public:
    explicit RuntimeURLRequestDelegate(base::WeakPtr<Runtime>& ptr) : ptr_(ptr) {}
    virtual ~RuntimeURLRequestDelegate() {}
    virtual void OnSuccess(base::ScopedPtr<base::PlatformString> url, base::ScopedPtr<base::PlatformString> response) {
        if (ptr_.IsValid()) {
            ptr_.Get()->RunScript(response);
        }
    }
    virtual void OnFailed(base::ScopedPtr<base::PlatformString> url, base::ScopedPtr<base::PlatformString> error) {}

 private:
    base::WeakPtr<Runtime> ptr_;
};
}  // namespace jscore
#endif  // LYNX_RUNTIME_RUNTIME_URL_REQUEST_DELEGATE_H_
