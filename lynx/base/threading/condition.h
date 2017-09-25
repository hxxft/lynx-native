// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_BASE_THREADING_CONDITION_H_
#define LYNX_BASE_THREADING_CONDITION_H_

#include "base/threading/lock.h"
#include "base/timer/time_utils.h"

namespace base {
class Condition {
 public:
    Condition() {
        pthread_cond_init(&condition_, NULL);
    }

    ~Condition() {
        pthread_cond_destroy(&condition_);
    }

    void Wait() {
        pthread_cond_wait(&condition_, &lock_.mutex_);
    }

    void Wait(uint64_t time) {
        timespec next_time = ToTimeSpecFromNow(time);
        pthread_cond_timedwait(&condition_, &lock_.mutex_, &next_time);
    }

    void Signal() {
        pthread_cond_signal(&condition_);
    }

 private:
    Lock lock_;
    pthread_cond_t condition_;
};
}  // namespace base

#endif  // LYNX_BASE_THREADING_CONDITION_H_
