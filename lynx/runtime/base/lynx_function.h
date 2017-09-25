// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_RUNTIME_LYNX_FUNCTION_H_
#define LYNX_RUNTIME_LYNX_FUNCTION_H_

#include <string>

namespace jscore {

class JSContext;
class LynxArray;

enum class TargetState : int {
    Empty = 0,
    Global = -1
};

class LynxFunction {
 public:
    LynxFunction(JSContext* context) : context_(context){}
    virtual ~LynxFunction() {}
    virtual void Run(void* target, LynxArray* args) {}

    std::string GetKey() { return js_function_key_;}

 protected:
    std::string js_function_key_;
    JSContext* context_;
};
}  // namespace jscore

#endif  // LYNX_RUNTIME_LYNX_FUNCTION_H_
