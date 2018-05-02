// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_RUNTIME_V8_V8_JS_FUNCTION_H_
#define LYNX_RUNTIME_V8_V8_JS_FUNCTION_H_

#include <cstdint>

#include "v8.h"

#include "runtime/base/lynx_function.h"

namespace jscore {

class V8Context;

class V8JSFunction : public LynxFunction {
 public:
    V8JSFunction(V8Context* context,
        v8::Local<v8::Object> target,
        v8::Local<v8::Value> function);
    virtual ~V8JSFunction();
    virtual void Run(void* target, void* args);

};
}  // namespace jscore

#endif  // LYNX_RUNTIME_V8_V8_JS_FUNCTION_H_
