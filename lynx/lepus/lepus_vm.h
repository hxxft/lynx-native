#ifndef LYNX_LEPUS_LEPUS_VM_H_
#define LYNX_LEPUS_LEPUS_VM_H_

#include "lepus/vm.h"

namespace lepus {
    class Context {
    private:
        void ExecuteFrame();
        std::vector<Value> memory_;
    };
    
    class VM {
    public:
        void Excute(Context* context);
    private:
        Context* current_context_;
    };
}

#endif
