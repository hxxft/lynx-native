// Copyright 2014 The Lynx Authors. All rights reserved.

#ifndef LYNX_BASE_THREADING_READ_WRITE_LOCK_H_
#define LYNX_BASE_THREADING_READ_WRITE_LOCK_H_

#include <pthread.h>
#include "condition.h"

namespace base {
    class ReadWriteLock {
    public:
        ReadWriteLock() : lock_(),
                          condition_(),
                          readers_(0),
                          waiting_writers_(0),
                          is_write_locked_(false){}

        ~ReadWriteLock() {}

        void ReadLock() {
            AutoLock lock(lock_);
            while (is_write_locked_ || waiting_writers_) {
                condition_.Wait();
            }
            readers_++;
        }

        void ReadUnlock() {
            AutoLock lock(lock_);
            if (!--readers_) {
                condition_.Broadcast();
            }
        }

        void WriteLock() {
            AutoLock lock(lock_);
            while (readers_ || is_write_locked_) {
                waiting_writers_++;
                condition_.Wait();
                waiting_writers_--;
            }
            is_write_locked_ = true;
        }

        void WriteUnlock() {
            AutoLock lock(lock_);
            is_write_locked_ = false;
            condition_.Broadcast();
        }

    private:
        Lock lock_;
        Condition condition_;
        unsigned int readers_;
        unsigned int waiting_writers_;
        bool is_write_locked_;
    };
}  // namespace base

#endif  // LYNX_BASE_THREADING_READ_WRITE_LOCK_H_
