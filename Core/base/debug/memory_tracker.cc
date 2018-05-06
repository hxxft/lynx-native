// Copyright 2017 The Lynx Authors. All rights reserved.

#include "base/debug/memory_tracker.h"

#include "base/log/logging.h"
#include "base/debug/backtrace.h"

#include <set>

namespace base {
    static MemoryTracker g_memory_tracker_;

    MemoryTracker* MemoryTracker::Instance() {
        return &g_memory_tracker_;
    }

    MemoryTracker::MemoryTracker() {

    }

    MemoryTracker::~MemoryTracker() {

    }

    void MemoryTracker::AddMemInfo(intptr_t ptr, const char* file, int line) {
        AutoLock lock(lock_);
        TrackerInfo tracker;
        tracker.size_ = debug::CaptureBacktrace(tracker.buffer_, 30);
        tracker.file_ = file;
        tracker.line_ = line;
        tracker_map_[ptr] = tracker;

    }

    void MemoryTracker::RemoveMemInfo(intptr_t ptr) {
        AutoLock lock(lock_);
        std::map<intptr_t, TrackerInfo>::iterator iter = tracker_map_.find(ptr);
        if(iter == tracker_map_.end()) {
#if OS_ANDROID
            debug::PrintCurrentBacktrace();
#endif
            return;
        }
        tracker_map_.erase(iter);
    }

    void MemoryTracker::PrintMemInfo() {
        AutoLock lock(lock_);
        std::map<intptr_t, TrackerInfo>::iterator iter = tracker_map_.begin();
        for(; iter != tracker_map_.end(); ++iter) {
            DLOG(ERROR) << "Memory Leak: " << iter->second.file_ << ":" << iter->second.line_;
#if OS_ANDROID
            debug::PrintBacktrace(iter->second.buffer_, iter->second.size_);
#endif
        }
        tracker_map_.clear();
    }
    
}
