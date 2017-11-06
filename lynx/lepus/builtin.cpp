
#include "lepus/builtin.h"
#include "lepus/vm_context.h"
#include "lepus/string.h"

namespace lepus {
    void Print(Context* context) {
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
                default:
                    break;
            }
        }
    }
    
    void RegisterCFunction(Context* context, const char* name, void* function) {
        Value value;
        value.native_function_ = function;
        value.type_ = ValueT_CFunction;
        context->SetGlobalData(context->string_pool().NewString(name), value);
    }
    
    void RegisterBulitin(Context* context) {
        RegisterCFunction(context, "print", reinterpret_cast<void*>(&Print));
    }
}
