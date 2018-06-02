// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_BASE_THREADING_MESSAGE_PUMP_POSIX_H_
#define LYNX_BASE_THREADING_MESSAGE_PUMP_POSIX_H_

#include "base/task/task.h"
#include "base/threading/condition.h"
#include "base/threading/message_pump.h"
#include "base/timer/timer.h"

namespace base {

class MessagePumpPosix : public MessagePump {
 public:
    MessagePumpPosix();

    virtual ~MessagePumpPosix();

    virtual void Run(Delegate* delegate);

    virtual void ScheduleWork();

    virtual void ScheduleDelayedWork(Closure* closure, int delayed_time);

    virtual void ScheduleIntervalWork(Closure* closure, int delayed_time);
    
    virtual void Stop();
 private:
    Condition condition_;

    Timer timer_;

    bool keep_running_;
};
}  // namespace base

#endif  // LYNX_BASE_THREADING_MESSAGE_PUMP_POSIX_H_
