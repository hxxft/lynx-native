// Copyright 2017 The Lynx Authors. All rights reserved.

#include "base/threading/message_loop.h"
#include "base/threading/message_pump_posix.h"
#include "base/threading/message_pump_io_posix.h"
#include "base/debug/memory_debug.h"

#if OS_ANDROID
#include "base/task/task.h"
#include "base/threading/message_pump_android.h"
#else
#include "base/threading/message_pump_ios.h"
#endif

#include "base/lazy_instance.h"
#include "base/threading/thread_local.h"

namespace base {
    
LazyInstance<ThreadLocalPointer<MessageLoop>> lazy_tls_ptr;

MessageLoop::MessageLoop(MESSAGE_LOOP_TYPE type) :
    lock_(),
    loop_type_(type),
    pump_(CreatePump(type)),
    weak_ptr_(this) {
        
}
    
void MessageLoop::BindToCurrentThread() {
    lazy_tls_ptr.Get()->Set(this);
}
    
MessageLoop* MessageLoop::current() {
    return lazy_tls_ptr.Get()->Get();
}

MessagePump* MessageLoop::CreatePump(MESSAGE_LOOP_TYPE type) {
    MessagePump* pump = NULL;
    switch (type) {
    case MESSAGE_LOOP_POSIX:
        pump = lynx_new MessagePumpPosix();
        break;
    case MESSAGE_LOOP_UI:
#if OS_ANDROID
        pump = lynx_new MessagePumpAndroid(this);
#else
        pump = lynx_new MessagePumpIOS(this);
#endif
        break;
    case MESSAGE_LOOP_IO:
        pump = lynx_new MessagePumpIOPosix();
        break;
    default:
        break;
    }
    return pump;
}

void MessageLoop::PostTask(Clouse* clouse) {
    AutoLock lock(lock_);
    Task task(clouse);
    incoming_task_queue_.push(task);
    pump_->ScheduleWork();
}

void MessageLoop::PostDelayedTask(Clouse* clouse, int delayed_time) {
    pump_->ScheduleDelayedWork(clouse, delayed_time);
}

void MessageLoop::PostIntervalTask(Clouse* clouse, int delayed_time) {
    pump_->ScheduleIntervalWork(clouse, delayed_time);
}

bool MessageLoop::DoWork() {
    loop_running_ = true;
    while (loop_running_) {
        {
            AutoLock lock(lock_);
            if(quit_task_.IsValid()) {
                loop_running_ = false;
                break;
            }
            incoming_task_queue_.Swap(&working_task_queue_);
            if (working_task_queue_.empty())
                break;
        }
        while (!working_task_queue_.empty()) {
            Task task = working_task_queue_.front();
            task.Run();
            working_task_queue_.pop();
        }
    }
    return loop_running_;
}

    void MessageLoop::DoQuit() {
        quit_task_.Run();
    }

    void MessageLoop::Quit(base::Clouse* closue) {
        AutoLock lock(lock_);
        quit_task_.Reset(closue);
        pump_->ScheduleWork();
    }

void MessageLoop::Run() {
    pump_->Run(this);
}

    void MessageLoop::Stop() {
        pump_->Stop();
    }
}  // namespace base
