// Copyright 2017 The Lynx Authors. All rights reserved.

#include "base/threading/message_pump_posix.h"

namespace base {

MessagePumpPosix::MessagePumpPosix() : condition_(), loop_running_(true) {}

MessagePumpPosix::~MessagePumpPosix() {}

void MessagePumpPosix::ScheduleWork() {
    condition_.Signal();
}

void MessagePumpPosix::ScheduleDelayedWork(Closure* closure, int delayed_time) {
    TimerNode node(closure, delayed_time);
    timer_.SetTimerNode(node);
    condition_.Signal();
}

void MessagePumpPosix::ScheduleIntervalWork(Closure* closure, int delayed_time) {
    TimerNode node(closure, delayed_time, true);
    timer_.SetTimerNode(node);
    condition_.Signal();
}

void MessagePumpPosix::Run(Delegate *delegate) {
    while (loop_running_) {
        timer_.Loop();
        loop_running_ = delegate->DoWork();
        if(loop_running_) {
            condition_.Wait(timer_.NextTimeout() - CurrentTimeMillis());
        }
    }
    delegate->DoQuit();
}
}  // namespace base
