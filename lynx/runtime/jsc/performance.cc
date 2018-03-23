//
// Created by dli on 9/19/16.
//

#include <base/print.h>
#include "Performance.h"

namespace jscore {
    Performance::Performance(const char* log) {
        log_ = log;
        gettimeofday(&time_, nullptr);
        use_micro_second_ = true;
        LOGD("xcore-timing", "%s start",log_);
    }

    Performance::Performance(const char* log, bool use_micro_second) {
        log_ = log;
        use_micro_second_ = use_micro_second;
        gettimeofday(&time_, nullptr);
        LOGD("xcore-timing", "%s start",log_);
    }

    Performance::~Performance() {
        timeval end;
        gettimeofday(&end, nullptr);

        if(use_micro_second_) {
            long micros_start = (time_.tv_sec * 1000 * 1000) + (time_.tv_usec);
            long micros_end = (end.tv_sec * 1000 * 1000) + (end.tv_usec);
            long interval = micros_end - micros_start;
            LOGD("xcore-timing", "%s end(%ldus)",log_, interval);
        } else {
            long micros_start = (time_.tv_sec * 1000 * 1000) + (time_.tv_usec);
            long micros_end = (end.tv_sec * 1000 * 1000) + (end.tv_usec);
            long interval = micros_end - micros_start;
            LOGD("xcore-timing", "%s end(%lfms)",log_, interval/1000.0);
        }
    }
}