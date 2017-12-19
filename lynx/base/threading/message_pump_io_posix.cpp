// Copyright 2017 The Lynx Authors. All rights reserved.

#include "base/threading/message_pump_io_posix.h"
#include "base/poller/select_poller.h"
#include "base/poller/utility.h"

#include <unistd.h>

namespace base {

MessagePumpIOPosix::MessagePumpIOPosix()
    : loop_running_(true),
    wakeup_pipe_in_(-1),
    wakeup_pipe_out_(-1),
    poller_(new SelectPoller){
        Init();
    }

MessagePumpIOPosix::~MessagePumpIOPosix() {
    if (wakeup_pipe_in_ >= 0) {
        close(wakeup_pipe_in_);
    }
    if (wakeup_pipe_out_ >= 0) {
        close(wakeup_pipe_out_);
    }
    poller_->RemoveFileDescriptor(wakeup_pipe_out_);
}

void MessagePumpIOPosix::ScheduleWork() {
    char buf = 0;
    //int nwrite = write(wakeup_pipe_in_, &buf, 1);
    write(wakeup_pipe_in_, &buf, 1);
}

void MessagePumpIOPosix::ScheduleDelayedWork(Clouse* clouse, int delayed_time) {
    TimerNode node(clouse, delayed_time);
    timer_.SetTimerNode(node);
    ScheduleWork();
}

void MessagePumpIOPosix::ScheduleIntervalWork(Clouse* clouse, int delayed_time) {
    TimerNode node(clouse, delayed_time, true);
    timer_.SetTimerNode(node);
    ScheduleWork();
}

void MessagePumpIOPosix::Run(Delegate *delegate) {
    while (loop_running_) {
        timer_.Loop();
        loop_running_ = delegate->DoWork();
        if(loop_running_) {
            uint64_t next_timeout = timer_.NextTimeout();
            uint64_t current_time = CurrentTimeMillis();
            if(next_timeout > current_time) {
                poller_->Poll(next_timeout - current_time);
            }
        }
    }
    delegate->DoQuit();
}
    
void MessagePumpIOPosix::OnFileCanRead(int fd) {
    char buf;
    int nread = read(fd, &buf, 1);
}
    
void MessagePumpIOPosix::OnFileCanWrite(int fd) {
    
}

bool MessagePumpIOPosix::Init() {
    int fds[2];
    if (pipe(fds)) {
//        DLOG(ERROR) << "pipe() failed, errno: " << errno;
        return false;
    }
    if (!SetNonBlocking(fds[0])) {
//        DLOG(ERROR) << "SetNonBlocking for pipe fd[0] failed, errno: " << errno;
        return false;
    }
    if (!SetNonBlocking(fds[1])) {
//        DLOG(ERROR) << "SetNonBlocking for pipe fd[1] failed, errno: " << errno;
        return false;
    }
    wakeup_pipe_out_ = fds[0];
    wakeup_pipe_in_ = fds[1];
    
    poller_->WatchFileDescriptor(new FileDescriptor(this, wakeup_pipe_out_, FD_EVENT_IN));
    return true;
}
}  // namespace base
