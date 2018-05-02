// Copyright 2017 The Lynx Authors. All rights reserved.

#include "loader/script/script_request_delegate.h"
#include "loader/script/script_loader.h"

namespace loader {
ScriptRequestDelegate::ScriptRequestDelegate(ScriptLoader* loader, const std::string& url):
    loader_(loader), url_(url) {
    loader_->AddRef();
}

ScriptRequestDelegate::~ScriptRequestDelegate() {
    loader_->Release();
}

void ScriptRequestDelegate::OnSuccess(base::ScopedPtr<base::PlatformString> url,
                                      base::ScopedPtr<base::PlatformString> response) {
    loader_->LoadScriptOnJSThread(url_, response->ToString());
}

void ScriptRequestDelegate::OnFailed(base::ScopedPtr<base::PlatformString> url,
                                     base::ScopedPtr<base::PlatformString> error) {
}

}  // namespace loader
