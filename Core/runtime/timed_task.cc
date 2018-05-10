// Copyright 2017 The Lynx Authors. All rights reserved.

#include "runtime/timed_task.h"
#include "runtime/global.h"
#include "runtime/js/js_context.h"
#include "base/debug/memory_debug.h"

namespace jscore {
    

    void TimedTaskInvoker::SetTimeout(JSContext* context, LynxFunction* function, int time) {
        context->runtime()->thread_manager()->RunOnJSThreadDelay(lynx_new TimedTask(function), time);
    }
    
    void TimedTaskInvoker::SetInterval(JSContext* context, LynxFunction* function, int time) {
        context->runtime()->thread_manager()->RunOnJSThreadInterval(lynx_new TimedTask(function), time);
    }
    
    void TimedTaskInvoker::clearTimeout() {
    }
    
    void TimedTask::Run() {
        if (js_function_.Get() != NULL) {
            js_function_->Run(js_function_->context()->global(), NULL);
        }
    }
}