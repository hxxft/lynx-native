
#include "lepus/vm.h"

#include <vector>
#include <string>

namespace lepus {
    void VM::Execute(Context* context, const std::string& source) {
        context->Execute(source);
    }
    
    Value VM::Call(Context* context, const std::string& name, const std::vector<Value>& args) {
        return context->Call(name, args);
    }
}
