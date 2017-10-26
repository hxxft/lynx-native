#ifndef LYNX_LEPUS_FUNCTION_H_
#define LYNX_LEPUS_FUNCTION_H_

#include <vector>

#include "lepus/op_code.h"
#include "lepus/value.h"
#include "lepus/syntax_tree.h"

namespace lepus {
    class Function {
    private:
        std::vector<Instruction> op_codes_;
        
        std::vector<Value> const_values_;
        
        std::vector<Function*> child_functions_;
        
    };
}

#endif  // LYNX_LEPUS_FUNCTION_H_
