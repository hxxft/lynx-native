// Copyright 2017 The Lynx Authors. All rights reserved.

#include <base/print.h>
#include "base/debug/timing_tracker.h"

namespace base {
    TimeingTracker::TimeingTracker(const char* log) {
        log_ = log;
        gettimeofday(&time_, nullptr);
    }

    TimeingTracker::~TimeingTracker() {
        timeval end;
        gettimeofday(&end, nullptr);

        long micros_start = (time_.tv_sec * 1000 * 1000) + (time_.tv_usec);
        long micros_end = (end.tv_sec * 1000 * 1000) + (end.tv_usec);
        long interval = micros_end - micros_start;
        LOGD("lynx-timing", "%s end(%lfms)",log_, interval/1000.0);
    }
}