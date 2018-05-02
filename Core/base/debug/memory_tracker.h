// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef ANDROID_MEMORY_TRACKER_H
#define ANDROID_MEMORY_TRACKER_H


#include <string>
#include <map>

#include "base/threading/lock.h"

namespace base {

    class MemoryTracker {
    public:
        static MemoryTracker *Instance();

        MemoryTracker();
        ~MemoryTracker();

        void AddMemInfo(intptr_t ptr, const char* file, int line);

        void RemoveMemInfo(intptr_t ptr);

        void PrintMemInfo();

    private:
        struct TrackerInfo {
            size_t size_;
            void* buffer_[30];
            std::string file_;
            int line_;
        };

        Lock lock_;
        std::map<intptr_t, TrackerInfo> tracker_map_;
    };
}


#endif //ANDROID_MEMORY_TRACKER_H
