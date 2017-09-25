// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_LOADER_SCRIPT_SCRIPT_LOADER_H_
#define LYNX_LOADER_SCRIPT_SCRIPT_LOADER_H_

#include <string>
#include <queue>
#include <map>

#include "base/scoped_ptr.h"
#include "net/url_request.h"
#include "loader/loader.h"

namespace jscore {
    class Runtime;

};

namespace loader {
    class HTMLRequestDelegate;
    class ScriptLoader : public LynxLoader {
    public:
        explicit ScriptLoader(jscore::Runtime* runtime, bool is_load_script_allowed);
        virtual ~ScriptLoader();

        virtual void Load(const std::string& url, int type);
        virtual void Flush();
        void LoadScriptOnJSThread(const std::string& url, const std::string& data);
    private:
        bool LoadFromCache(const std::string& url);

        jscore::Runtime* runtime_;
        std::queue<std::string> script_queue_;
        std::map<std::string, std::string> script_map_;
        bool is_load_script_allowed_;
    };
}  // namespace loader

#endif  // LYNX_LOADER_SCRIPT_SCRIPT_LOADER_H_