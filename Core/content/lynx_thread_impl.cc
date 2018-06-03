// Copyright 2017 The Lynx Authors. All rights reserved.

#include "content/lynx_thread.h"
#include "base/lazy_instance.h"

namespace content {

namespace {
    static const char* g_lynx_thread_names[LynxThread::ID_COUNT] = {
        "",
        "Lynx_IOThread",
        "Lynx_JSThread",
        "Lynx_PluginThread",
    };

    static const base::MessageLoop::MESSAGE_LOOP_TYPE g_lynx_thread_loop_type[LynxThread::ID_COUNT] = {
        base::MessageLoop::MESSAGE_LOOP_NONE,
        base::MessageLoop::MESSAGE_LOOP_NONE,
        base::MessageLoop::MESSAGE_LOOP_NONE,
        base::MessageLoop::MESSAGE_LOOP_POSIX,
    };

    class ThreadManager {
        public:
        void Initialize() {
            for(int i = 0; i < LynxThread::ID_COUNT; ++i) {
                if(g_lynx_thread_loop_type[i] != base::MessageLoop::MESSAGE_LOOP_NONE) {
                    threads[i] = lynx_new base::Thread(g_lynx_thread_loop_type[i], g_lynx_thread_names[i]);
                    threads[i]->Start();
                }
            }
        }
        void PostTask(LynxThread::ID identifier, base::Closure* task) {
            threads[identifier]->Looper()->PostTask(task);
        }
        private:
        base::Thread* threads[LynxThread::ID_COUNT];
    };

    base::LazyInstance<ThreadManager> g_thread_manager;
}

void LynxThread::Initialize() {
    g_thread_manager.Get()->Initialize();
}

void LynxThread::PostTask(ID identifier, base::Closure* task) {
    g_thread_manager.Get()->PostTask(identifier, task);
}

}

