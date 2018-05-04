// Copyright 2017 The Lynx Authors. All rights reserved.

#include "base/debug/timing_tracker.h"
#include "base/log/logging.h"

namespace base {
    TimingTracker::TimingTracker(const char* log) {
        log_ = log;
        start_ = std::chrono::high_resolution_clock::now();
    }

    TimingTracker::~TimingTracker() {
        auto finish = std::chrono::high_resolution_clock::now();
        double cost = std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start_).count()
                      / 1000000.0;
        DLOG(INFO)<<"lynx-timing: "<<log_<<" end("<<cost<<"fms)";
    }
}