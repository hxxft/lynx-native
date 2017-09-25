// Copyright 2017 The Lynx Authors. All rights reserved.

#include "net/url_request_ghost_delegate.h"
#include "net/url_request_context.h"
#include "runtime/runtime.h"

namespace net {

    URLRequestGhostDelegate::URLRequestGhostDelegate(URLRequestContext* context,
                                                     URLRequest* request,
                                                     URLRequestDelegate* delegate) :
            URLRequestDelegate(), delegate_(delegate), context_(context),
            request_(request), weak_ptr_(this) {
        this->AddRef();
    }

    URLRequestGhostDelegate::~URLRequestGhostDelegate() {
        weak_ptr_.Invalidate();
    }

    void URLRequestGhostDelegate::DeliverSuccess(base::PlatformString& url,
                                                 base::PlatformString& response) {
        context_->runtime()
                ->thread_manager()
                ->RunOnJSThread(
                        base::Bind(&URLRequestGhostDelegate::OnSuccess,
                                   weak_ptr_,
                                   url,
                                   response));
    }

    void URLRequestGhostDelegate::DeliverError(base::PlatformString &url,
                                               base::PlatformString &error) {
        context_->runtime()
                ->thread_manager()
                ->RunOnJSThread(
                        base::Bind(&URLRequestGhostDelegate::OnFailed,
                                   weak_ptr_,
                                   url,
                                   error));
    }

    void URLRequestGhostDelegate::OnSuccess(const base::PlatformString &url,
                                            const base::PlatformString &response) {
        delegate_->OnSuccess(url, response);
        context_->Remove(request_);
        this->Die();
    }

    void URLRequestGhostDelegate::OnFailed(const base::PlatformString &url,
                                           const base::PlatformString &error) {
        delegate_->OnFailed(url, error);
        context_->Remove(request_);
        this->Die();
    }

    void URLRequestGhostDelegate::Die() {
        this->Release();
    }
}