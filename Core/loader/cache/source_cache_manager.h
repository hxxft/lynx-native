// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_LOADER_CACHE_SOURCE_CACHE_MANAGER_H_
#define LYNX_LOADER_CACHE_SOURCE_CACHE_MANAGER_H_

#include <map>
#include <string>
#include <base/threading/lock.h>

#include "base/scoped_ptr_map.h"
#include "loader/cache/source_cache.h"

namespace loader {
    class SourceCacheManager {
    public:
        bool HasCache(const std::string &key);
        bool ReadCache(const std::string& key, std::string& source, int type);
        void WriteCache(const std::string& key, const std::string& source, int type);
    private:
        typedef base::ScopedPtrMap<std::string, loader::SourceCache> SourceCacheMap;
        SourceCacheMap source_caches_;
        base::Lock lock_;
    };
}  // namespace loader


#endif  // LYNX_LOADER_CACHE_SOURCE_CACHE_MANAGER_H_
