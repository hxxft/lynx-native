#ifndef LYNX_LEPUS_MATH_API_H_
#define LYNX_LEPUS_MATH_API_H_

namespace lepus {
    void RegisterMathAPI(Context* ctx) {
        Dictonary* table = new Dictonary;
        RegisterFunctionTable(ctx, "Math", table);
    }
}

#endif
