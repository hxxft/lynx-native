// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef BASE_DEBUG_TIMING_PERFORMANCE_H_
#define BASE_DEBUG_TIMING_PERFORMANCE_H_


#include <chrono>

namespace base {
    class TimingTracker {
    public:
        TimingTracker(const char *log);
        ~TimingTracker();

    private:
        std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds> start_;
        const char *log_;
    };
}

#endif  // BASE_DEBUG_TIMING_PERFORMANCE_H_
