// Copyright 2017 The Lynx Authors. All rights reserved.

#include "loader/html/html_request_delegate.h"

namespace loader {
HTMLRequestDelegate::HTMLRequestDelegate(HTMLLoader* loader, const std::string& url):
                                    loader_(loader), url_(url) {
    loader_->AddRef();
}

HTMLRequestDelegate::~HTMLRequestDelegate() {
    loader_->Release();
}

void HTMLRequestDelegate::OnSuccess(const base::PlatformString& url,
                                    const base::PlatformString& response) {
    loader_->LoadHTML(url_, response.ToString());
}

void HTMLRequestDelegate::OnFailed(const base::PlatformString& url,
                                   const base::PlatformString& error) {
}

}  // namespace loader
