
#include "lepus/builtin.h"
#include "lepus/vm_context.h"
#include "lepus/lepus_string.h"

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

    void RegisterCFunction(Context* context, const char* name, void* function) {
        Value value;
        value.native_function_ = function;
        value.type_ = ValueT_CFunction;
        context->SetGlobalData(context->string_pool()->NewString(name), value);
    }
    
    void RegisterBulitin(Context* ctx) {
        RegisterCFunction(ctx, "print", reinterpret_cast<void*>(&Print));
    }
}
