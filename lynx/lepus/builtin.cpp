
#include "lepus/builtin.h"
#include "lepus/vm_context.h"
#include "lepus/string.h"

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
        RegisterCFunction(ctx, "setTranslateY", reinterpret_cast<void*>(&Print));
        RegisterCFunction(ctx, "setTranslateX", reinterpret_cast<void*>(&Print));
        RegisterCFunction(ctx, "setScaleX", reinterpret_cast<void*>(&Print));
        RegisterCFunction(ctx, "setScaleY", reinterpret_cast<void*>(&Print));
        RegisterCFunction(ctx, "setRotateX", reinterpret_cast<void*>(&Print));
        RegisterCFunction(ctx, "setRotateY", reinterpret_cast<void*>(&Print));
        RegisterCFunction(ctx, "setPivotX", reinterpret_cast<void*>(&Print));
        RegisterCFunction(ctx, "setPivotY", reinterpret_cast<void*>(&Print));
        RegisterCFunction(ctx, "setAlpha", reinterpret_cast<void*>(&Print));
        RegisterCFunction(ctx, "setOffsetTop", reinterpret_cast<void*>(&Print));
        RegisterCFunction(ctx, "setOffsetBottom", reinterpret_cast<void*>(&Print));
        RegisterCFunction(ctx, "setOffsetLeft", reinterpret_cast<void*>(&Print));
        RegisterCFunction(ctx, "setOffsetRight", reinterpret_cast<void*>(&Print));
        RegisterCFunction(ctx, "setConsumed", reinterpret_cast<void*>(&Print));
        
    }
}
