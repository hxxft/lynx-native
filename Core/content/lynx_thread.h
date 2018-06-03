// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_CORE_CONTENT_LYNX_THREAD_H_
#define LYNX_CORE_CONTENT_LYNX_THREAD_H_

#include "base/threading/thread.h"

namespace content {
    class LynxThread {
        public:
        enum ID {
            UI,
            IO,
            JS,
            PLUGIN,
            ID_COUNT,
        };
        static void Initialize();
        static void PostTask(ID identifier, base::Closure* task);
    };
}  // namespace content

#endif  // LYNX_CORE_CONTENT_LYNX_THREAD_H_
