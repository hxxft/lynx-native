
#ifndef LYNX_LEPUS_BUILTIN_H_
#define LYNX_LEPUS_BUILTIN_H_

#include "lepus/context.h"
#include "lepus/table.h"

namespace lepus {
    void RegisterBulitin(Context* context);
    void RegisterCFunction(Context* context, const char* name, CFunction function);
    void RegisterFunctionTable(Context* context, const char* name, void* function);
    void RegisterTableFunction(Context* context, Dictonary* table, const char* name, CFunction function);
}

#endif
