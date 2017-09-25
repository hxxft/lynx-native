// Copyright 2017 The Lynx Authors. All rights reserved.
#include "loader/script/script_loader.h"

#include "config/global_config_data.h"
#include "loader/script/script_request_delegate.h"
#include "net/url_request_context.h"
#include "net/url_parser.h"
#include "runtime/runtime.h"

namespace loader {


    ScriptLoader::ScriptLoader(jscore::Runtime* runtime, bool is_load_script_allowed)
            : runtime_(runtime), is_load_script_allowed_(is_load_script_allowed) {
    }

    ScriptLoader::~ScriptLoader() {

    }

    void ScriptLoader::Load(const std::string& url, int type) {
        script_queue_.push(url);
        if(!LoadFromCache(url)) {
            runtime_->url_requset_context()
                    ->CreateRequest(url, lynx_new ScriptRequestDelegate(this, url))
                    ->Fetch();
        }
    }

    void ScriptLoader::Flush() {
        is_load_script_allowed_ = true;
        while(!script_queue_.empty()) {
            std::map<std::string, std::string>::iterator iter = script_map_.find(script_queue_.front());
            if(iter == script_map_.end())
                break;
            script_queue_.pop();
            runtime_->LoadScript(iter->second);
        }
    }

    void ScriptLoader::LoadScriptOnJSThread(const std::string& url, const std::string& data) {
        if (!config::GlobalConfigData::GetInstance()->cache_manager().HasCache(url)) {
            config::GlobalConfigData::GetInstance()->cache_manager().WriteCache(url, data, SCRIPT_FILE);
        }
        script_map_[url] = data;
        if(is_load_script_allowed_) {
            while(!script_queue_.empty()) {
                std::map<std::string, std::string>::iterator iter = script_map_.find(script_queue_.front());
                if(iter == script_map_.end())
                    break;
                script_queue_.pop();
                runtime_->LoadScript(iter->second);
            }
        }
    }

    bool ScriptLoader::LoadFromCache(const std::string& url) {
//        std::string data;
//        if(config::GlobalConfigData::GetInstance()->cache_manager().ReadCache(url, data, SCRIPT_FILE)) {
//            base::ScopedRefPtr<ScriptLoader> ref(this);
//            runtime_->thread_manager()->RunOnJSThread(
//                    base::Bind(&ScriptLoader::LoadScriptOnJSThread, ref, url, data));
//            return true;
//        }
        return false;
    }
}