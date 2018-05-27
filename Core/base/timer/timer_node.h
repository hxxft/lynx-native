// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_BASE_TIMER_TIMER_NODE_H_
#define LYNX_BASE_TIMER_TIMER_NODE_H_

#include "base/task/task.h"
#include "base/timer/time_utils.h"

namespace base {

class TimerNode {
 public:
    TimerNode(Closure* closure, int interval_time)
        : task_(closure),
        interval_time_(interval_time),
        need_loop_(false) {
        next_timeout_ = CurrentTimeMillis() + interval_time_;
    }

    TimerNode(Closure* closure, int interval_time, bool need_loop)
        : task_(closure),
        interval_time_(interval_time),
        need_loop_(need_loop) {
        next_timeout_ = CurrentTimeMillis() + interval_time_;
    }

    TimerNode(TimerNode& other) :
        task_(other.task_),
        interval_time_(other.interval_time_),
        need_loop_(other.need_loop_),
        next_timeout_(other.next_timeout_) {
    }

    TimerNode(const TimerNode& other) :
        task_(other.task_),
        interval_time_(other.interval_time_),
        need_loop_(other.need_loop_),
        next_timeout_(other.next_timeout_) {
    }

    Task task_;
    int interval_time_;
    bool need_loop_;
    uint64_t next_timeout_;
};
}  // namespace base

#endif  // LYNX_BASE_TIMER_TIMER_NODE_H_
