// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_BASE_THREADING_THREAD_H_
#define LYNX_BASE_THREADING_THREAD_H_

#include <pthread.h>

#include "base/threading/message_loop.h"

namespace base {

class Thread {
 public:
    class ThreadParams {
     public:
        explicit ThreadParams(Thread* thread): thread_(thread) {}
        Thread* thread_;
    };

    explicit Thread(MessageLoop::MESSAGE_LOOP_TYPE type);

    virtual ~Thread();

    virtual void Start();

    virtual void Quit(base::Clouse* closue);

    void Run();

    void Join(const Thread& thread);

    MessageLoop* Looper() { return &message_loop_; }

    static long int CurrentId() {
#if OS_IOS
        return pthread_mach_thread_np(pthread_self());
#elif OS_ANDROID
        return pthread_self();
#endif
    }

 private:
    void ThreadMain();
    pthread_t thread_handle_;
    MessageLoop message_loop_;
};

}  // namespace base
#endif  // LYNX_BASE_THREADING_THREAD_H_
