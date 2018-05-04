// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_NET_URL_REQUEST_H_
#define LYNX_NET_URL_REQUEST_H_

#include <string>
#include "net/url_request_delegate.h"
#include "net/url_request_ghost_delegate.h"
#include "base/debug/memory_debug.h"

namespace net {
class URLRequestContext;
class URLRequest {
 public:
    URLRequest(URLRequestContext *context,
               const std::string &url,
               URLRequestDelegate *delegate)
            : context_(context),
              ghost_delegate_(lynx_new URLRequestGhostDelegate(context,
                                                          this,
                                                          delegate)) {

    }
    virtual ~URLRequest() {
    }
    
    static URLRequest* Create(URLRequestContext *context,
                              const std::string &url,
                              URLRequestDelegate *delegate);
    
    // ThreadSafe
    virtual void Fetch() {}

    // NotThreadSafe
    // Should be called on UI thread.
    virtual void Cancel() {
        ghost_delegate_->Die();
    }

protected:
    URLRequestContext* context_;
    URLRequestGhostDelegate* ghost_delegate_;
};
}
#endif  // LYNX_NET_URL_REQUEST_H_
