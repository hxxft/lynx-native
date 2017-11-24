#ifndef LYNX_LEPUS_BASE_API_H_
#define LYNX_LEPUS_BASE_API_H_

#include "lepus/builtin.h"
#include "lepus/vm_context.h"
#include "lepus/table.h"

namespace lepus {
    Value Print(Context* context) {
        int params_count = context->GetParamsSize();
        for(int i = 0; i < params_count; i++) {
            Value* v = context->GetParam(i);
            switch (v->type_) {
                case Value_Nil:
                    printf("null\n");
                    break;
                case Value_Number:
                    printf("%lf\n", v->number_);
                    break;
                case Value_Boolean:
                    printf("%s\n", v->boolean_ ? "true" : "false");
                    break;
                case Value_String:
                    printf("%s\n", v->str_->c_str());
                    break;
                default:
                    break;
            }
        }
        return Value();
    }
    
    void RegisterBaseAPI(Context* ctx) {
        Dictonary* table = new Dictonary;
        RegisterTableFunction(ctx, table, "log", &Print);
        RegisterFunctionTable(ctx, "console", table);
    }
}

#endif

