// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_LOADER_HTML_HTML_LOADER_H_
#define LYNX_LOADER_HTML_HTML_LOADER_H_

#include <string>
#include <queue>
#include <map>

#include "base/scoped_ptr.h"
#include "net/url_request.h"
#include "loader/loader.h"
#include "loader/script/script_loader.h"

namespace jscore {
    class Runtime;

};

namespace loader {
    class HTMLRequestDelegate;
    class HTMLLoader : public LynxLoader {
    public:
        explicit HTMLLoader(jscore::Runtime* runtime);
        virtual ~HTMLLoader();

        virtual void Load(const std::string& url, int type);
        virtual void Flush();
        void LoadHTML(const std::string& url, const std::string& data);
    private:

        void LoadHTMLOnUIThread(const std::string& url, const std::string& data);
        bool LoadFromCache(const std::string& url);

        jscore::Runtime* runtime_;
        std::string html_url_;
        base::ScopedRefPtr<ScriptLoader> script_loader_;
    };
}  // namespace loader

#endif  // LYNX_LOADER_HTML_HTML_LOADER_H_