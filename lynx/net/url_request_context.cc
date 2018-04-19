// Copyright 2017 The Lynx Authors. All rights reserved.

#include "net/url_request_context.h"
#include "net/url_request.h"
#include "net/url_request_delegate.h"
#include "runtime/runtime.h"

namespace net {
    
    URLRequestContext::URLRequestContext(jscore::Runtime* runtime)
            : runtime_(runtime), lock_(), stop_flag_(false) {
    }

    URLRequestContext::~URLRequestContext() {
    }
    
    URLRequest* URLRequestContext::CreateRequest(const std::string& url,
                                                 URLRequestDelegate* delegate) {
        URLRequest* request = URLRequest::Create(this, url, delegate);
        Add(request);
        return request;
    }
    
    void URLRequestContext::Add(URLRequest* request) {
        base::AutoLock lock(lock_);
        if (stop_flag_) {
            request->Cancel();
        }
        request_vector_.push_back(request);
    }
    
    void URLRequestContext::Remove(URLRequest* request) {
        base::AutoLock lock(lock_);
        auto it = request_vector_.begin();
        for (; it != request_vector_.end(); it++) {
            if (*it == request) {
                break;
            }
        }
        if (it != request_vector_.end()) {
            request_vector_.erase(it);
        }
    }

    void URLRequestContext::Cancel(URLRequest* request) {
        base::AutoLock lock(lock_);
        auto it = request_vector_.begin();
        for (; it != request_vector_.end(); it++) {
            if (*it == request) {
                break;
            }
        }
        if (it != request_vector_.end()) {
            request->Cancel();
            request_vector_.erase(it);
        }
    }
    
    void URLRequestContext::CancelAll() {
        base::AutoLock lock(lock_);
        auto it = request_vector_.begin();
        for (; it != request_vector_.end(); it++) {
            (*it)->Cancel();
        }
        request_vector_.clear();
    }

    void URLRequestContext::Stop() {
        {
            base::AutoLock lock(lock_);
            stop_flag_ = true;
        }
        CancelAll();
    }
}
