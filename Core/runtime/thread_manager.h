// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_RUNTIME_THREAD_MANAGER_H_
#define LYNX_RUNTIME_THREAD_MANAGER_H_

#include "base/debug/memory_debug.h"
#include "base/scoped_ptr.h"
#include "base/threading/thread.h"

namespace jscore {
class ThreadManager {
 public:
  ThreadManager()
      : js_thread_(lynx_new base::Thread(base::MessageLoop::MESSAGE_LOOP_POSIX,
                                         "JSThread")),
        ui_thread_(lynx_new base::Thread(base::MessageLoop::MESSAGE_LOOP_UI)) {
    js_thread_->Start();
  }

  void RunOnUIThread(base::Closure* closure) {
    ui_thread_->Looper()->PostTask(closure);
  }

  void RunOnJSThread(base::Closure* closure) {
    js_thread_->Looper()->PostTask(closure);
  }

  void RunOnJSThreadDelay(base::Closure* closure, int delayed_time) {
    js_thread_->Looper()->PostDelayedTask(closure, delayed_time);
  }

  void RunOnJSThreadInterval(base::Closure* closure, int delayed_time) {
    js_thread_->Looper()->PostIntervalTask(closure, delayed_time);
  }

  void QuitJSThread(base::Closure* closure) { js_thread_->Quit(closure); }

  void DetachUIThread() { ui_thread_->Looper()->Stop(); }

  base::Thread* js_thread() { return js_thread_.Get(); }

  base::Thread* ui_thread() { return ui_thread_.Get(); }

 private:
  base::ScopedPtr<base::Thread> js_thread_;
  base::ScopedPtr<base::Thread> ui_thread_;
};
}  // namespace jscore

#endif  // LYNX_RUNTIME_THREAD_MANAGER_H_
