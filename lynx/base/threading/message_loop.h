// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_BASE_THREADING_MESSAGE_LOOP_H_
#define LYNX_BASE_THREADING_MESSAGE_LOOP_H_

#if OS_ANDROID
#include <queue>
#endif

#include "base/threading/message_pump.h"
#include "base/threading/message_pump_io_posix.h"

#include "base/scoped_ptr.h"
#include "base/task/task.h"
#include "base/threading/condition.h"
#include "base/threading/lock.h"

namespace base {
class MessageLoop : public MessagePump::Delegate {
 public:
    enum MESSAGE_LOOP_TYPE {
        MESSAGE_LOOP_UI,
        MESSAGE_LOOP_POSIX,
        MESSAGE_LOOP_IO,
    };
    explicit MessageLoop(MESSAGE_LOOP_TYPE type = MESSAGE_LOOP_POSIX);
    void PostTask(Clouse* clouse);
    void PostDelayedTask(Clouse* clouse, int delayed_time);
    void PostIntervalTask(Clouse* clouse, int delayed_time);
    virtual bool DoWork();
    virtual void DoQuit();
    void Run();
    void Quit(base::Clouse* closue);
    void Stop();
    void BindToCurrentThread();
    
    MessagePump* pump(){
        return pump_.Get();
    }
    
    static MessageLoop* current();
 private:
    MessagePump* CreatePump(MESSAGE_LOOP_TYPE type);
    TaskQueue incoming_task_queue_;
    TaskQueue working_task_queue_;
    bool loop_running_;
    Task quit_task_;

    Lock lock_;
    MESSAGE_LOOP_TYPE loop_type_;
    ScopedPtr<MessagePump> pump_;
    WeakPtr<MessageLoop> weak_ptr_;
};

    class MessageLoopForIO : public MessageLoop {
    public:
        MessageLoopForIO() : MessageLoop(MESSAGE_LOOP_IO) {
            
        }
        
        static MessageLoopForIO* current() {
            MessageLoop* loop = MessageLoop::current();
            return static_cast<MessageLoopForIO*>(loop);
        }
        
        void WatchFileDescriptor(FileDescriptor* descriptor) {
            static_cast<MessagePumpIOPosix*>(pump())->poller()->WatchFileDescriptor(descriptor);
        }
        
        void RemoveFileDescriptor(int fd) {
            static_cast<MessagePumpIOPosix*>(pump())->poller()->RemoveFileDescriptor(fd);
        }
        
    private:
        MESSAGE_LOOP_TYPE type_;
    };
}  // namespace base

#endif  // LYNX_BASE_THREADING_MESSAGE_LOOP_H_
