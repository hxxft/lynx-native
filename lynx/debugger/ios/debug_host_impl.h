// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_DEBUGGER_DEBUG_HOST_IMPL_H_
#define LYNX_DEBUGGER_DEBUG_HOST_IMPL_H_

#include "debugger/debug_host.h"
#include "base/threading/thread.h"

#import <Foundation/Foundation.h>

@interface DebugHostImplBridge : NSObject

@end

namespace debug {
    class DebugHostImpl : public DebugHost {
    public:
        DebugHostImpl();
        ~DebugHostImpl();
        virtual void Run(DebugType type) override;
    private:
        DebugHostImplBridge* bridge_;
    };
}

#endif  // LYNX_DEBUGGER_DEBUG_HOST_H_
