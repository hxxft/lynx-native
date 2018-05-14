#ifndef LYNX_LEPUS_MATH_API_H_
#define LYNX_LEPUS_MATH_API_H_
#include <time.h>

namespace lepus {
    
    Value Sin(Context* context) {
        Value* arg = context->GetParam(1);
        if(arg->type_ != Value_Number){
            return Value();
        }
        return Value(sin(arg->number_));
    }
    Value Acos(Context* context) {
        Value* arg = context->GetParam(1);
        if(arg->type_ != Value_Number){
            return Value();
        }
        return Value(acos(arg->number_));
    }
    
    Value Asin(Context* context) {
        Value* arg = context->GetParam(1);
        if(arg->type_ != Value_Number){
            return Value();
        }
        return Value(asin(arg->number_));
    }
    
    Value Abs(Context* context) {
        Value* arg = context->GetParam(1);
        if(arg->type_ != Value_Number){
            return Value();
        }
        return Value(fabs(arg->number_));
    }
    
    Value Atan(Context* context) {
        Value* arg = context->GetParam(1);
        if(arg->type_ != Value_Number){
            return Value();
        }
        return Value(atan(arg->number_));
    }
    Value Ceil(Context* context) {
        Value* arg = context->GetParam(1);
        if(arg->type_ != Value_Number){
            return Value();
        }
        return Value(ceil(arg->number_));
    }
    Value Cos(Context* context) {
        Value* arg = context->GetParam(1);
        if(arg->type_ != Value_Number){
            return Value();
        }
        return Value(cos(arg->number_));
    }
    Value Exp(Context* context) {
        Value* arg = context->GetParam(1);
        if(arg->type_ != Value_Number){
            return Value();
        }
        return Value(exp(arg->number_));
    }
    Value Floor(Context* context) {
        Value* arg = context->GetParam(1);
        if(arg->type_ != Value_Number){
            return Value();
        }
        return Value(floor(arg->number_));
    }
    Value Log(Context* context) {
        Value* arg = context->GetParam(1);
        if(arg->type_ != Value_Number){
            return Value();
        }
        return Value(log(arg->number_));
    }
    Value Max(Context* context) {
        Value* arg1 = context->GetParam(1);
        Value* arg2 = context->GetParam(2);
        if(arg1->type_ != Value_Number||arg2->type_ != Value_Number){
            return Value();
        }
        return Value(fmax(arg1->number_, arg2->number_));
    }
    Value Min(Context* context) {
        Value* arg1 = context->GetParam(1);
        Value* arg2 = context->GetParam(2);
        if(arg1->type_ != Value_Number||arg2->type_ != Value_Number){
            return Value();
        }
        return Value(fmin(arg1->number_, arg2->number_));
    }
    Value Pow(Context* context) {
        Value* arg1 = context->GetParam(1);
        Value* arg2 = context->GetParam(2);
        if(arg1->type_ != Value_Number||arg2->type_ != Value_Number){
            return Value();
        }
        return Value(pow(arg1->number_, arg2->number_));
    }
    Value Random(Context* context) {
        static bool seeded = false;
        if (!seeded) {
            seeded = true;
            srand(static_cast<unsigned int>(time(NULL)));
        }
        return Value((float)rand()/RAND_MAX);
    }
    Value Round(Context* context) {
        Value* arg = context->GetParam(1);
        if(arg->type_ != Value_Number){
            return Value();
        }
        return Value(round(arg->number_));
    }
    Value Sqrt(Context* context) {
        Value* arg = context->GetParam(1);
        if(arg->type_ != Value_Number){
            return Value();
        }
        return Value(sqrt(arg->number_));
    }
    Value Tan(Context* context) {
        Value* arg = context->GetParam(1);
        if(arg->type_ != Value_Number){
            return Value();
        }
        return Value(tan(arg->number_));
    }
    
    
    void RegisterMathAPI(Context* ctx) {
        Dictonary* table = lynx_new Dictonary;
        RegisterTableFunction(ctx, table, "sin", &Sin);
        RegisterTableFunction(ctx, table, "abs", &Abs);
        RegisterTableFunction(ctx, table, "acos", &Acos);
        RegisterTableFunction(ctx, table, "atan", &Atan);
        RegisterTableFunction(ctx, table, "asin", &Asin);
        RegisterTableFunction(ctx, table, "ceil", &Ceil);
        RegisterTableFunction(ctx, table, "cos", &Cos);
        RegisterTableFunction(ctx, table, "exp", &Exp);
        RegisterTableFunction(ctx, table, "floor", &Floor);
        RegisterTableFunction(ctx, table, "log", &Log);
        RegisterTableFunction(ctx, table, "max", &Max);
        RegisterTableFunction(ctx, table, "min", &Min);
        RegisterTableFunction(ctx, table, "pow", &Pow);
        RegisterTableFunction(ctx, table, "random", &Random);
        RegisterTableFunction(ctx, table, "round", &Round);
        RegisterTableFunction(ctx, table, "sqrt", &Sqrt);
        RegisterTableFunction(ctx, table, "tan", &Tan);
        RegisterFunctionTable(ctx, "Math", table);
    }
}

#endif
