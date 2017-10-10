// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef BASE_DEBUG_TIMING_PERFORMANCE_H_
#define BASE_DEBUG_TIMING_PERFORMANCE_H_


#include<sys/time.h>
namespace base {
    class TimeingTracker {
    public:
        TimeingTracker(const char *log);
        ~TimeingTracker();

    private:
        timeval time_;
        const char *log_;
    };
}

#endif  // BASE_DEBUG_TIMING_PERFORMANCE_H_
