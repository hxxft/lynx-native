// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_BASE_SCOPED_PTR_MAP_H_
#define LYNX_BASE_SCOPED_PTR_MAP_H_

#include <unordered_map>
#include <utility>
#include "base/debug/memory_debug.h"

#if !GNU_SUPPORT && OS_ANDROID
#define unordered_map tr1::unordered_map
#endif

namespace base {
template<class Key, class Ptr>
class ScopedPtrMap {
 public:
    typedef typename std::unordered_map<Key, Ptr*>::iterator iterator;
    ScopedPtrMap() {}
    ~ScopedPtrMap() {
        clear();
    }

    iterator begin() { return map_.begin(); }
    iterator end() { return map_.end(); }

    iterator find(const Key& key) { return map_.find(key); }

    std::pair<iterator, bool> add(const Key& key, Ptr* ptr) {
        std::pair<iterator, bool> result =
                    map_.insert(std::make_pair(key, ptr));
        return result;
    }

    void erase(iterator iter) {
        lynx_delete(iter->second);
        map_.erase(iter);
    }

    void clear() {
        for (iterator iter = map_.begin(); iter != map_.end(); ++iter) {
            lynx_delete(iter->second);
        }
        map_.clear();
    }

    bool empty() { return map_.empty(); }

    int size() { return map_.size(); }

    Ptr*& operator[](const Key& key) {
        return map_[key];
    }

 private:
    std::unordered_map<Key, Ptr*> map_;
};
}  // namespace base

#endif  // LYNX_BASE_SCOPED_PTR_MAP_H_
