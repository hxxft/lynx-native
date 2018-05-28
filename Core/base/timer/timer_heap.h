// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_BASE_TIMER_TIMER_HEAP_H_
#define LYNX_BASE_TIMER_TIMER_HEAP_H_

#include <vector>

#include "base/timer/timer_node.h"
#include "base/threading/lock.h"

namespace base {
class TimerHeap {
 public:
    TimerHeap(): lock_() {}
    ~TimerHeap() {}
    uint64_t NextTimeout() { return !min_heap_.empty() ? min_heap_[0].next_timeout_ : ULLONG_MAX; }
    bool IsEmpty() { return min_heap_.empty(); }
    void Remove();
    TimerNode Pop();
    void Push(TimerNode node);
 private:
    void ShiftUp(int start);
    void ShiftDown(int start);

    std::vector<TimerNode> min_heap_;

    Lock lock_;
};
}  // namespace base

#endif  // LYNX_BASE_TIMER_TIMER_HEAP_H_
