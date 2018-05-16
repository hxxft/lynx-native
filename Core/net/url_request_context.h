// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_NET_URL_REQUEST_CONTEXT_H_
#define LYNX_NET_URL_REQUEST_CONTEXT_H_

#include <string>
#include "base/scoped_vector.h"
#include "base/weak_ptr.h"
#include "base/ref_counted_ptr.h"
#include "base/threading/lock.h"

namespace jscore {
    class Runtime;
}

namespace net {
class URLRequest;
class URLRequestDelegate;
class URLRequestContext {
public:
    URLRequestContext(jscore::Runtime* runtime);
    virtual ~URLRequestContext();

    // ThreadSafe
    URLRequest* CreateRequest(const std::string& url, URLRequestDelegate* delegate);

    jscore::Runtime* runtime() {
        return runtime_;
    }
    // ThreadSafe
    void Add(URLRequest* request);
    // ThreadSafe
    void Remove(URLRequest* request);

    // NotThreadSafe
    // Should be called on UI thread.
    void Cancel(URLRequest* request);
    // NotThreadSafe
    // Should be called on UI thread.
    void CancelAll();
    // NotThreadSafe
    // Should be called on UI thread.
    void Stop();

private:
    
    base::ScopedVector<URLRequest> request_vector_;
    jscore::Runtime* runtime_;
    base::Lock lock_;
    bool stop_flag_;

};
}  // namespace net
#endif  // LYNX_NET_URL_REQUEST_CONTEXT_H_
