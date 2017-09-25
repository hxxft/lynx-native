// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_NET_URL_REQUEST_PLATFORM_DELEGATE_H_
#define LYNX_NET_URL_REQUEST_PLATFORM_DELEGATE_H_

#include "net/url_request_delegate.h"
#include <string>
#include "base/weak_ptr.h"
#include "base/ref_counted_ptr.h"

namespace net {
class URLRequestContext;
class URLRequest;
class URLRequestGhostDelegate : public URLRequestDelegate {
 public:
    explicit URLRequestGhostDelegate(URLRequestContext* context,
                                     URLRequest* request,
                                     URLRequestDelegate* delegate);

    virtual ~URLRequestGhostDelegate();

    // NotThreadSafe
    // Should be called on UI thread.
    void DeliverSuccess(base::PlatformString& url,
                        base::PlatformString& response);

    // NotThreadSafe
    // Should be called on UI thread.
    void DeliverError(base::PlatformString& url,
                      base::PlatformString& error);

    // Will be called on JS thread
    virtual void OnSuccess(const base::PlatformString& url,
                           const base::PlatformString& response);

    // Will be called on JS thread
    virtual void OnFailed(const base::PlatformString& url,
                          const base::PlatformString& error);

    // NotThreadSafe
    // Should be called on UI thread.
    void Die();

 private:

    base::ScopedRefPtr<URLRequestDelegate> delegate_;
    base::WeakPtr<URLRequestGhostDelegate> weak_ptr_;
    URLRequestContext* context_;
    URLRequest* request_;
};
}  // namespace net
#endif  // LYNX_NET_URL_REQUEST_PLATFORM_DELEGATE_H_
