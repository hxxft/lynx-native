
#ifndef LYNX_LEPUS_BUILTIN_H_
#define LYNX_LEPUS_BUILTIN_H_

#include "lepus/context.h"

namespace lepus {
    void RegisterBulitin(Context* context);
    void RegisterCFunction(Context* context, const char* name, void* function);
}

#endif
