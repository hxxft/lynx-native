// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_BASE_TIMER_TIME_UTILS_H_
#define LYNX_BASE_TIMER_TIME_UTILS_H_

#include <sys/time.h>

#if OS_IOS
#include <cstdint>
#endif

namespace base {
uint64_t CurrentTimeMillis();
timespec ToTimeSpecFromNow(uint64_t interval_time);
}  // namespace base

#endif  // LYNX_BASE_TIMER_TIME_UTILS_H_
