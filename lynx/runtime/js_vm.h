// Copyright 2017 The Lynx Authors. All rights reserved.


#ifndef LYNX_RUNTIME_JS_VM_H_
#define LYNX_RUNTIME_JS_VM_H_

#include <map>

#include "base/ref_counted_ptr.h"

namespace jscore {
class JSVM : public base::RefCountPtr<JSVM>{
public:
    JSVM(): vm_(NULL) {}
    ~JSVM();
    void Initialize();

    void* vm() {
        return vm_;
    }
private:
    void* vm_;
};
}  // namespace jscore

#endif  // LYNX_RUNTIME_JS_VM_H_
