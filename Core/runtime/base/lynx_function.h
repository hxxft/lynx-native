// Copyright 2017 The Lynx Authors. All rights reserved.

#ifndef LYNX_RUNTIME_LYNX_FUNCTION_H_
#define LYNX_RUNTIME_LYNX_FUNCTION_H_

#include <string>

namespace jscore {

class JSContext;
class LynxArray;
class LynxObject;

class LynxFunction {
 public:
    LynxFunction(JSContext* context) : context_(context){}
    virtual ~LynxFunction() {}
    virtual void Run(LynxObject* target, LynxArray* args) {}

    std::string GetKey() { return js_function_key_;}

    inline JSContext* context() {
        return context_;
    }

 protected:
    std::string js_function_key_;
    JSContext* context_;
};
}  // namespace jscore

#endif  // LYNX_RUNTIME_LYNX_FUNCTION_H_
