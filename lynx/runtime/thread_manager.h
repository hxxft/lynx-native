// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_RUNTIME_THREAD_MANAGER_H_
#define LYNX_RUNTIME_THREAD_MANAGER_H_

#include "base/threading/thread.h"
#include "base/scoped_ptr.h"
#include "base/debug/memory_debug.h"

namespace jscore {
class ThreadManager {
 public:
    ThreadManager() :
        js_thread_(lynx_new base::Thread(base::MessageLoop::MESSAGE_LOOP_POSIX)),
        ui_thread_(lynx_new base::Thread(base::MessageLoop::MESSAGE_LOOP_UI)) {
        js_thread_->Start();
    }

    void RunOnUIThread(base::Clouse* clouse) {
        ui_thread_->Looper()->PostTask(clouse);
    }

    void RunOnJSThread(base::Clouse* closue) {
        js_thread_->Looper()->PostTask(closue);
    }

    void RunOnJSThreadDelay(base::Clouse* closue, int delayed_time) {
        js_thread_->Looper()->PostDelayedTask(closue, delayed_time);
    }

    void RunOnJSThreadInterval(base::Clouse* closue, int delayed_time) {
        js_thread_->Looper()->PostIntervalTask(closue, delayed_time);
    }

    void QuitJSThread(base::Clouse* closue) {
        js_thread_->Quit(closue);
    }

    void DetachUIThread() {
        ui_thread_->Looper()->Stop();
    }

 private:
    base::ScopedPtr<base::Thread> js_thread_;
    base::ScopedPtr<base::Thread> ui_thread_;
};
}  // namespace jscore

#endif  // LYNX_RUNTIME_THREAD_MANAGER_H_
