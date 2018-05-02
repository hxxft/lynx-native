// Copyright 2017 The Lynx Authors. All rights reserved.

#include "loader/cache/source_cache_manager.h"
#include "base/debug/memory_debug.h"

namespace loader {

    bool SourceCacheManager::HasCache(const std::string& key) {
        base::AutoLock lock(lock_);
        SourceCacheMap::iterator iter = source_caches_.find(key);
        return !(iter == source_caches_.end());
    }

    bool SourceCacheManager::ReadCache(const std::string& key, std::string& source, int type) {
        base::AutoLock lock(lock_);
        SourceCacheMap::iterator iter = source_caches_.find(key);
        if(iter == source_caches_.end()) {
            return false;
        }
        iter->second->ReadCache(key, source, type);
        return true;
    }

    void SourceCacheManager::WriteCache(const std::string& key, const std::string& source, int type) {
        base::AutoLock lock(lock_);
        SourceCache* cache = NULL;
        SourceCacheMap::iterator iter = source_caches_.find(key);
        if(iter == source_caches_.end()) {
            cache = lynx_new SourceCache();
            source_caches_.add(key, cache);
        }else{
            cache = iter->second;
        }
        cache->WriteCache(key, source, type);
    }
}
