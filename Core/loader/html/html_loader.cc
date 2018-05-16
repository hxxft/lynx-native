// Copyright 2017 The Lynx Authors. All rights reserved.

#include "config/global_config_data.h"
#include "base/debug/memory_debug.h"
#include "net/url_request_context.h"
#include "net/url_parser.h"
#include "loader/html/html_loader.h"
#include "runtime/runtime.h"
#include "loader/html/html_request_delegate.h"

namespace loader {

    HTMLLoader::HTMLLoader(jscore::Runtime* runtime)
            : runtime_(runtime), script_loader_(lynx_new ScriptLoader(runtime, false)) {
    }

    HTMLLoader::~HTMLLoader() {

    }

    void HTMLLoader::Load(const std::string& data, int type) {
        switch (type) {
            case MAIN_FILE:
                html_url_ = data;
                if (!LoadFromCache(data)) {
                    runtime_->url_requset_context()
                            ->CreateRequest(data, lynx_new HTMLRequestDelegate(this, data))
                            ->Fetch();
                }
                break;
            case SCRIPT_FILE:
//                std::string url = ToCompleteUrl(data, html_url_);
                script_loader_->Load(data, type);
                break;
        }
    }

    void HTMLLoader::Flush() {
        script_loader_->Flush();
    }

    void HTMLLoader::LoadHTML(const std::string& url, const std::string& data) {
        config::GlobalConfigData::GetInstance()->cache_manager().WriteCache(url, data, MAIN_FILE);
        base::ScopedRefPtr<HTMLLoader> ref(this);
        runtime_->thread_manager()->RunOnUIThread(
                base::Bind(&HTMLLoader::LoadHTMLOnUIThread, ref, url, data));
    }

    void HTMLLoader::LoadHTMLOnUIThread(const std::string& url, const std::string& data) {
        runtime_->LoadHTML(data);
    }

    bool HTMLLoader::LoadFromCache(const std::string& url) {
        return false;
//        std::string data;
//        if(config::GlobalConfigData::GetInstance()->cache_manager().ReadCache(url, data, MAIN_FILE)) {
//            LoadHTMLOnUIThread(url, data);
//            return true;
//        }
//        return false;
    }
}
