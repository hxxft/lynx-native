// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_BASE_TASK_TASK_H_
#define LYNX_BASE_TASK_TASK_H_

#include <queue>

#include "base/scoped_ptr.h"
#include "base/task/callback.h"
#include "base/threading/condition.h"

namespace base {
class Task {
 public:
    explicit Task(Closure* closure) : runnable_(closure) {}

    Task(Task& other) : runnable_(other.runnable_) {}

    Task(const Task& other) : runnable_(const_cast<Task*>(&other)->runnable_) {}

    Task() {}

    ~Task() {}

    void Reset(Closure* closure) {
      runnable_.Reset(closure);
    }

    void Run() { runnable_->Run(); }

    bool IsValid() {
        return runnable_.Get();
    }

 private:
    ScopedPtr<Closure> runnable_;
};

class TaskQueue : public std::queue<Task> {
 public:
    void Swap(TaskQueue* queue) {
        c.swap(queue->c);
    }
};
}  // namespace base

#endif  // LYNX_BASE_TASK_TASK_H_
