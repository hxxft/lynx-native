// Copyright 2017 The Lynx Authors. All rights reserved.

#include "base/timer/timer.h"

namespace base {

void Timer::Loop() {

    std::vector<TimerNode> vec;
    while (!timer_heap_.IsEmpty()
            && timer_heap_.NextTimeout() <= CurrentTimeMillis()) {
        TimerNode node = timer_heap_.Pop();
        node.task_.Run();
        if(node.need_loop_) {
            vec.push_back(node);
        }
    }
    for(int i = 0; i < vec.size(); i++) {
        TimerNode node = vec[i];
        node.next_timeout_ += node.interval_time_;
        timer_heap_.Push(node);
    }
    vec.clear();
}

}  // namespace base
