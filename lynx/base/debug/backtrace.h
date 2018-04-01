// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_BASE_BEBUG_BACKTRACE_H_
#define LYNX_BASE_BEBUG_BACKTRACE_H_



#include <iostream>
#include <iomanip>

#include <unwind.h>
#include <dlfcn.h>
#include <sstream>

#include "base/log/logging.h"

namespace {

    struct BacktraceState
    {
        void** current;
        void** end;
    };

    static _Unwind_Reason_Code unwindCallback(struct _Unwind_Context* context, void* arg)
    {
        BacktraceState* state = static_cast<BacktraceState*>(arg);
        uintptr_t pc = _Unwind_GetIP(context);
        if (pc) {
            if (state->current == state->end) {
                return _URC_END_OF_STACK;
            } else {
                *state->current++ = reinterpret_cast<void*>(pc);
            }
        }
        return _URC_NO_REASON;
    }

}
namespace base {
    namespace debug {
        size_t CaptureBacktrace(void** buffer, size_t max)
        {
            BacktraceState state = {buffer, buffer + max};
#if OS_ANDROID
            _Unwind_Backtrace(unwindCallback, &state);
#endif

            return state.current - buffer;
        }

        void DumpBacktrace(void** buffer, size_t count)
        {
            for (size_t idx = 0; idx < count; ++idx) {
                const void* addr = buffer[idx];
                const char* symbol = "";

                Dl_info info;
                if (dladdr(addr, &info) && info.dli_sname) {
                    symbol = info.dli_sname;
                }

                DLOG(WARNING) << "#" << idx << ":" << symbol;
            }
        }


        void PrintCurrentBacktrace() {
            const size_t max = 30;
            void* buffer[max];

            size_t size = CaptureBacktrace(buffer, max);

            DumpBacktrace(buffer, size);
        }

        void PrintBacktrace(void** buffer, size_t count) {

            DumpBacktrace(buffer, count);
        }
    }
}

#endif //ANDROID_BACKTRACE_H
