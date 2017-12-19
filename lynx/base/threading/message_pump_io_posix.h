// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_BASE_THREADING_MESSAGE_PUMP_IO_POSIX_H_
#define LYNX_BASE_THREADING_MESSAGE_PUMP_IO_POSIX_H_

#include "base/task/task.h"
#include "base/threading/condition.h"
#include "base/threading/message_pump.h"
#include "base/timer/timer.h"
#include "base/poller/poller.h"
#include "base/scoped_ptr.h"

namespace base {

class MessagePumpIOPosix : public MessagePump, public Poller::Watcher {
 public:
    MessagePumpIOPosix();

    virtual ~MessagePumpIOPosix();

    virtual void Run(Delegate* delegate);

    virtual void ScheduleWork();

    virtual void ScheduleDelayedWork(Clouse* clouse, int delayed_time);

    virtual void ScheduleIntervalWork(Clouse* clouse, int delayed_time);

    virtual void OnFileCanRead(int fd);
    
    virtual void OnFileCanWrite(int fd);
    
    Poller* poller() {
        return poller_.Get();
    }
    
 private:
    
    bool Init();
    
    Timer timer_;

    bool loop_running_;
    
    int wakeup_pipe_in_;
    int wakeup_pipe_out_;
    
    ScopedPtr<Poller> poller_;
};
}  // namespace base

#endif  // LYNX_BASE_THREADING_MESSAGE_PUMP_IO_POSIX_H_
