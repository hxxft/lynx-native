// Copyright 2017 The Lynx Authors. All rights reserved.

#include "loader/cache/source_cache.h"
#include "loader/loader.h"

namespace loader {

SourceCache::SourceCache() : main_key_(), main_source_(), script_sources_() {

}

SourceCache::~SourceCache() {

}

bool SourceCache::ReadCache(const std::string& key, std::string& source,  int type) {
    if(type == LynxLoader::MAIN_FILE) {
        source = main_key_ == key ? main_source_ : "";
        return !source.empty();
    }else if(type == LynxLoader::SCRIPT_FILE) {
        return FindScriptSource(key, source);
    }
    return false;
}

void SourceCache::WriteCache(const std::string& key, const std::string& source, int type) {
    if(type == LynxLoader::MAIN_FILE) {
        main_key_ = key;
        main_source_ = source;
    }else if(type == LynxLoader::SCRIPT_FILE) {
        script_sources_[key] = source;
    }
}

bool SourceCache::FindScriptSource(const std::string& key, std::string& source) {
    ScriptSourceMap::iterator iter = script_sources_.find(key);
    if(iter != script_sources_.end()) {
        source = iter->second;
        return true;
    }
    return false;
}

}