// Copyright 2017 The Lynx Authors. All rights reserved.


#ifndef LYNX_RUNTIME_TIMED_TASK_INVOKER_H_
#define LYNX_RUNTIME_TIMED_TASK_INVOKER_H_

#include "base/task/callback.h"
#include "base/scoped_ptr.h"
#include "runtime/runtime.h"
#include "runtime/base/lynx_function.h"

namespace jscore {
    
    class JSContext;
    
    class TimedTask : public base::Closure{
    public:
        
        TimedTask(LynxFunction* js_function) : js_function_(js_function) {}
        virtual ~TimedTask() {}
        
    protected:
        virtual void Run();
        
    private:
        base::ScopedPtr<LynxFunction> js_function_;
    };
    
    class TimedTaskInvoker {
    public:
        void SetTimeout(JSContext* context, LynxFunction* function, int time);
        void SetInterval(JSContext* context, LynxFunction* function, int time);
        void clearTimeout();
    };
    
}


#endif // LYNX_RUNTIME_TIMED_TASK_INVOKER_H_
