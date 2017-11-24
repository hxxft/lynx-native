
#include "lepus/builtin.h"
#include "lepus/base_api.h"
#include "lepus/math_api.h"
#include "lepus/string_api.h"

namespace lepus {
    
    void RegisterCFunction(Context* context, const char* name, CFunction function) {
        Value value;
        value.native_function_ = reinterpret_cast<void*>(function);
        value.type_ = Value_CFunction;
        context->SetGlobalData(context->string_pool()->NewString(name), value);
    }
    
    void RegisterFunctionTable(Context* context, const char* name, void* table) {
        Value value;
        value.table_ = table;
        value.type_ = Value_Table;
        context->SetGlobalData(context->string_pool()->NewString(name), value);
    }
    
    void RegisterTableFunction(Context* context, Dictonary* table, const char* name, CFunction function) {
        Value value;
        value.native_function_ = reinterpret_cast<void*>(function);
        value.type_ = Value_CFunction;
        table->SetValue(context->string_pool()->NewString(name), value);
    }
    
    void RegisterBulitin(Context* ctx) {
        RegisterBaseAPI(ctx);
        RegisterStringAPI(ctx);
        RegisterMathAPI(ctx);
    }
}
