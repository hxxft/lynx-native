
#ifndef LYNX_LEPUS_VM_H_
#define LYNX_LEPUS_VM_H_

#include "lepus/value.h"
#include "lepus/context.h"

namespace lepus {

    class VM {
    public:
        ~VM() {}
        void Execute(Context* context, const std::string& source);
        Value Call(Context* context, const std::string& name, const std::vector<Value>& args);
    };
}

#endif  // LYNX_LEPUS_VM_H_
