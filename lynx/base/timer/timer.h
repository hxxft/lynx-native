// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_BASE_TIMER_TIMER_H_
#define LYNX_BASE_TIMER_TIMER_H_

#include "base/timer/timer_heap.h"

namespace base {
class Timer {
 public:
    void Loop();

    uint64_t NextTimeout() { return timer_heap_.NextTimeout(); }

    void SetTimerNode(TimerNode node) { timer_heap_.Push(node); }

 private:
    TimerHeap timer_heap_;
};
}  // namespace base

#endif  // LYNX_BASE_TIMER_TIMER_H_
