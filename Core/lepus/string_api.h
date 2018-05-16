#ifndef LYNX_LEPUS_STRING_API_H_
#define LYNX_LEPUS_STRING_API_H_

#include "lepus/lepus_string.h"
#include "lepus/builtin.h"
#include "lepus/vm_context.h"
#include "lepus/table.h"

namespace lepus {
    Value IndexOf(Context* context) {
        long params_count = context->GetParamsSize();
        Value* thiz = context->GetParam(0);
        Value* arg = context->GetParam(1);
        long index = params_count == 2 ? 0 : context->GetParam(2)->number_;
        
        if(thiz->type_ == Value_String && arg->type_ == Value_String) {
            return Value(thiz->str_->find(*arg->str_, index));
        }
        
        return Value(-1);
    }
    
    void RegisterStringAPI(Context* ctx) {
        Dictonary* table = lynx_new Dictonary;
        RegisterTableFunction(ctx, table, "indexOf", &IndexOf);
        RegisterFunctionTable(ctx, "String", table);
    }
}

#endif  // LYNX_LEPUS_STRING_API_H_

