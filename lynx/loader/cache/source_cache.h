// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_LOADER_CACHE_SOURCE_CACHE_H_
#define LYNX_LOADER_CACHE_SOURCE_CACHE_H_

#include <map>
#include <string>

namespace loader {
class SourceCache {
 public:
    SourceCache();
    ~SourceCache();
    bool ReadCache(const std::string& key, std::string& source, int type);
    void WriteCache(const std::string& key, const std::string& source, int type);
 private:
    bool FindScriptSource(const std::string& key, std::string& source);

    std::string main_key_;
    std::string main_source_;
    typedef std::map<std::string, std::string> ScriptSourceMap;
    ScriptSourceMap script_sources_;
};
}

#endif  // LYNX_LOADER_CACHE_SOURCE_CACHE_H_
