// Copyright 2014 The Lynx Authors. All rights reserved.

#ifndef LYNX_BASE_THREADING_LOCK_H_
#define LYNX_BASE_THREADING_LOCK_H_

#include <pthread.h>

namespace base {
class Lock {
 public:
    Lock() { pthread_mutex_init(&mutex_, NULL); }

    ~Lock() {
        pthread_mutex_destroy(&mutex_);
    }

    void Acquire() { pthread_mutex_lock(&mutex_); }

    void Release() { pthread_mutex_unlock(&mutex_); }

    friend class Condition;
    friend class CompletionEvent;
 private:
    pthread_mutex_t mutex_;
};

class AutoLock {
 public:
    explicit AutoLock(Lock& lock) : lock_(lock) {
        lock_.Acquire();
    }

    ~AutoLock() {
        lock_.Release();
    }

 private:
    Lock& lock_;
};
}  // namespace base

#endif  // LYNX_BASE_THREADING_LOCK_H_
